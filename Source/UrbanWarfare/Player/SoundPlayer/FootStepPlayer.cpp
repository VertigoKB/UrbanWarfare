// Fill out your copyright notice in the Description page of Project Settings.


#include "FootStepPlayer.h"
#include "Kismet/GameplayStatics.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/PlayerBehaviorComponent.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

void UFootStepPlayer::ExternalInitialize(APlayerBase* const InOwnerPawn)
{
	OwnerPawn = InOwnerPawn;
	if (OwnerPawn)
		World = OwnerPawn->GetWorld();
	else
	{
		LOG_EFUNC(TEXT("Initialization failed: OwnerPawn."));
	}

	if (World)
	{
		World->GetTimerManager().SetTimer(InitTimer, this, &UFootStepPlayer::InitConstruct, 0.1f, true);
	}
	else
	{
		LOG_EFUNC(TEXT("Initialization failed: World."));
	}
}

void UFootStepPlayer::BeginDestroy()
{
	Super::BeginDestroy();

	if (World)
		World->GetTimerManager().ClearAllTimersForObject(this);
}

void UFootStepPlayer::InitConstruct()
{
	if (InitCount < 0)
	{
		LOG_EFUNC(TEXT("Initialization failed: Unable to initialize. timer clear."));
		World->GetTimerManager().ClearTimer(InitTimer);
		return;
	}

	PlayerBehavior = OwnerPawn->GetPlayerBehavior();
	if (!PlayerBehavior)
	{
		LOG_EFUNC(TEXT("Initialization failed: PlayerBehavior. Retry:%d"), InitCount--);
		return;
	}

	BlueprintConfig = OwnerPawn->GetBlueprintConfig();
	if (!BlueprintConfig)
	{
		LOG_EFUNC(TEXT("Initialization failed: BlueprintConfig. Retry:%d"), InitCount--);
		return;
	}

	FootSteopRef = BlueprintConfig->FootStepsSound;
	if (!FootSteopRef)
	{
		LOG_EFUNC(TEXT("Initialization failed: FootSteopSound. Retry:%d"), InitCount--);
		return;
	}

	LoadedFootSteps = FootSteopRef.LoadSynchronous();
	if (!LoadedFootSteps)
	{
		LOG_EFUNC(TEXT("Initialization failed: LoadedFootSteps. Retry:%d"), InitCount--);
		return;
	}

	LOG_EFUNC(TEXT("The object successfully initialized."));
	bIsInitialized = true;
	World->GetTimerManager().ClearTimer(InitTimer);
}

void UFootStepPlayer::CalculatePlayerVelocity()
{
	if (!bIsInitialized)
		return;

	FVector Velocity = OwnerPawn->GetVelocity();
	float SpeedSquared = Velocity.SizeSquared2D();

	if (SpeedSquared >= MinVelocitySquared - KINDA_SMALL_NUMBER)
		PlayFootStepSound(SpeedSquared);
	else
	{
		if (bPlayingFootSteps)
			StopFootStepSound();
	}
}

void UFootStepPlayer::PlayFootStepSound(const float SpeedSquared)
{
	if (SpeedSquared < MinVelocitySquared || OwnerPawn->IsPlayerFalling())
	{
		StopFootStepSound();
		return;
	}

	if (PlayerBehavior->MovementState.LastState() == EMovementState::Running)
	{
		if (!(FMath::IsNearlyEqual(FootStepSoundInterval, RunInterval)))
			World->GetTimerManager().ClearTimer(FootStepHandle);
		FootStepSoundInterval = RunInterval;
	}
	else
	{
		if (!(FMath::IsNearlyEqual(FootStepSoundInterval, WalkInterval)))
			World->GetTimerManager().ClearTimer(FootStepHandle);
		FootStepSoundInterval = WalkInterval;
	}

	if (!bPlayingFootSteps)
	{
		bPlayingFootSteps = true;
		UGameplayStatics::PlaySoundAtLocation(World, LoadedFootSteps, OwnerPawn->GetActorLocation());
	}

	if (!(World->GetTimerManager().IsTimerActive(FootStepHandle)))
	{
		World->GetTimerManager().SetTimer(FootStepHandle, FTimerDelegate::CreateLambda([this]() {
			UGameplayStatics::PlaySoundAtLocation(World, LoadedFootSteps, OwnerPawn->GetActorLocation());
			}), FootStepSoundInterval, true);
	}
}

void UFootStepPlayer::StopFootStepSound()
{
	bPlayingFootSteps = false;
	World->GetTimerManager().ClearTimer(FootStepHandle);
}
