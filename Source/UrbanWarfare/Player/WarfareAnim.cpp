// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareAnim.h"
#include "KismetAnimationLibrary.h"

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
			}
			else
			{
				LOG_EFUNC(TEXT("Failed to cach ThePawn."))
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

bool UWarfareAnim::CacheAndInit()
{
	ThePlayer = Cast<APlayerBase>(TryGetPawnOwner());
	if (!ThePlayer)
		return false;

	PlayerBehavior = Cast<UPlayerBehaviorComponent>(ThePlayer->GetPlayerBehavior());
	if (!PlayerBehavior)
		return false;

	if (!PlayerBehavior->bInitFlag)
		return false;

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
