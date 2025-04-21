// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FootStepPlayer.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UFootStepPlayer : public UObject
{
	GENERATED_BODY()

public:
	void ExternalInitialize(class APlayerBase* const InOwnerPawn);
	virtual void BeginDestroy() override;
	void InitConstruct();

	void CalculatePlayerVelocity();

private:
	void PlayFootStepSound(const float SpeedSquared);
	void StopFootStepSound();
private:
	UPROPERTY()
	TObjectPtr<class UWorld> World;
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class UPlayerBehaviorComponent> PlayerBehavior;
	UPROPERTY()
	TSoftObjectPtr<USoundBase> FootSteopRef;
	UPROPERTY()
	TObjectPtr<class UBlueprintConfig> BlueprintConfig;
	UPROPERTY()
	USoundBase* LoadedFootSteps;

	float FootStepSoundInterval = RunInterval;
	float WalkInterval = 1.f;
	float RunInterval = 0.42f;
	float MinVelocitySquared = 40.f * 40.f;

	bool bPlayingFootSteps = false;
	bool bIsInitialized = false;

	FTimerHandle FootStepHandle;
	FTimerHandle InitTimer;
	int8 InitCount = 10;
};
