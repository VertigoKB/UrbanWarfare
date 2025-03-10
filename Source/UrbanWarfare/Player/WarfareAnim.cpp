// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareAnim.h"
#include "KismetAnimationLibrary.h"

#include "../Common/WarfareLogger.h"

UWarfareAnim::UWarfareAnim()
{
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

	FVector CurrentVelocity = ThePawn->GetVelocity();
	FRotator CurrentRotation = ThePawn->GetActorRotation();

	MovementSpeed = CurrentVelocity.Length();
	MovementDirection = UKismetAnimationLibrary::CalculateDirection(CurrentVelocity, CurrentRotation);
		
}

bool UWarfareAnim::CacheAndInit()
{
	ThePawn = TryGetPawnOwner();
	if (!ThePawn)
		return false;

	return true;
}
