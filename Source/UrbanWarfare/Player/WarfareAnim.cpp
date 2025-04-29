// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareAnim.h"
#include "KismetAnimationLibrary.h"

#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "../Common/WarfareLogger.h"

UWarfareAnim::UWarfareAnim()
{
	CurrentMovementState = EMovementState::Running;
}

void UWarfareAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!IsValid(GetWorld()) ||
		!(GetWorld()->IsGameWorld()))
		return;

	if (GetWorld())
	{
		TWeakObjectPtr<UWarfareAnim> WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([WeakThis]() {
			
			if (!(WeakThis.IsValid()))
				return;

			UWarfareAnim* This = WeakThis.Get();

			This->bInitFlag = This->CacheAndInit();
			if (This->bInitFlag)
			{
				This->GetWorld()->GetTimerManager().ClearTimer(This->InitTimer);
				LOG_EFUNC(TEXT("Success to initialize UWarfareAnim"));
			}
			}), 0.3f, true);
	}
}

void UWarfareAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!bInitFlag)
		return;

	SetMovementProperty();
	SetTransitionProperty();

}

void UWarfareAnim::PlayMontage_ReloadPistol()
{
	ensure(MontageReloadPistol);

	Montage_Stop(0.f);
	Montage_Play(MontageReloadPistol);
}

void UWarfareAnim::PlayMontage_ReloadRifle()
{
	ensure(MontageReloadRifle);

	Montage_Stop(0.f);
	Montage_Play(MontageReloadRifle);
}

void UWarfareAnim::PlayMontage_FirePistol()
{
	FName Who = this->GetFName();
	ensure(MontageFirePistol);

	Montage_Stop(0.f);
	Montage_Play(MontageFirePistol);
}

void UWarfareAnim::PlayMontage_FireRifle()
{
	FName Who = this->GetFName();
	ensure(MontageFireRifle);

	Montage_Stop(0.f);
	Montage_Play(MontageFireRifle);
}

bool UWarfareAnim::CacheAndInit()
{
	ThePlayer = Cast<APlayerBase>(TryGetPawnOwner());
	if (!ThePlayer)
	{
		LOG_EFUNC(TEXT("ThePlayer is null"));
		return false;
	}

	UBlueprintConfig* BlueprintConfig = ThePlayer->GetBlueprintConfig();

	if (ThePlayer->ActorHasTag("CounterTrist"))
	{
		MontageReloadPistol = BlueprintConfig->CounterTristReloadPistol;
		MontageReloadRifle = BlueprintConfig->CounterTristReloadRifle;
		MontageFirePistol = BlueprintConfig->CounterTristFirePistol;
		MontageFireRifle = BlueprintConfig->CounterTristFireRifle;
	}
	else if (ThePlayer->ActorHasTag("Terrorist"))
	{
		MontageReloadPistol = BlueprintConfig->TerroristReloadPistol;
		MontageReloadRifle = BlueprintConfig->TerroristReloadRifle;
		MontageFirePistol = BlueprintConfig->TerroristFirePistol;
		MontageFireRifle = BlueprintConfig->TerroristFireRifle;
	}
	else
	{
		LOG_EFUNC(TEXT("Failed to find tag"));
		return false;
	}

		 

	PlayerBehavior = ThePlayer->GetPlayerBehavior();
	if (!PlayerBehavior)
	{
		LOG_EFUNC(TEXT("PlayerBehavior is null"));
		return false;
	}

	if (!PlayerBehavior->bInitFlag)
	{
		LOG_EFUNC(TEXT("PlayerBehavior not initialied."));
		return false;
	}

	return true;
}

void UWarfareAnim::SetMovementProperty()
{
	FVector CurrentVelocity = ThePlayer->GetVelocity();
	FRotator CurrentRotation = ThePlayer->GetActorRotation();

	MovementSpeed = CurrentVelocity.Length();
	MovementDirection = UKismetAnimationLibrary::CalculateDirection(CurrentVelocity, CurrentRotation);
}

void UWarfareAnim::SetTransitionProperty()
{
	CurrentMovementState = PlayerBehavior->MovementState.LastState();
	bInAir = ThePlayer->IsPlayerFalling();
	
	if (EquippedWeapon !=
		ThePlayer->GetWeaponComponent()->GetEquippedWeaponType())
		EquippedWeapon = ThePlayer->GetWeaponComponent()->GetEquippedWeaponType();

	UpdateAimDirection();
}

void UWarfareAnim::UpdateAimDirection()
{
	float ServerAimDirection = PlayerBehavior->AimDirection;
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (FMath::Abs(ServerAimDirection - AimDirection) > 30.f)
		AimDirection = ServerAimDirection;

	AimDirection = FMath::FInterpConstantTo(AimDirection, ServerAimDirection, DeltaTime, 100.f);
}
