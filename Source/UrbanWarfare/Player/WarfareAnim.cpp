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

bool UWarfareAnim::CacheAndInit()
{
	ThePlayer = Cast<APlayerBase>(TryGetPawnOwner());
	if (!ThePlayer)
	{
		LOG_EFUNC(TEXT("ThePlayer is null"));
		return false;
	}

	if (ThePlayer->ActorHasTag("CounterTrist"))
	{
		MontageReloadPistol = ThePlayer->GetBlueprintConfig()->CounterTristReloadPistol;
		MontageReloadRifle = ThePlayer->GetBlueprintConfig()->CounterTristReloadRifle;
	}
	else if (ThePlayer->ActorHasTag("Terrorist"))
	{
		MontageReloadPistol = ThePlayer->GetBlueprintConfig()->TerroristReloadPistol;
		MontageReloadRifle = ThePlayer->GetBlueprintConfig()->TerroristReloadRifle;
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
