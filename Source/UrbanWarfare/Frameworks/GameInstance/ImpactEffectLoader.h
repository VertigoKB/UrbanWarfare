// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "ImpactEffectLoader.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FOnCompleteLoad_A)
DECLARE_DELEGATE(FOnCompleteLoad_B)
DECLARE_DELEGATE(FOnCompleteLoad_C)
DECLARE_DELEGATE(FOnCompleteLoad_D)
UCLASS()
class URBANWARFARE_API UImpactEffectLoader : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	bool LoadEffectData();
	class UImpactEffectData* GetImpactEffectData();

	UPROPERTY()
	TObjectPtr<UImpactEffectData> EffectData;

	bool bIsLoadSuccess = false;

private:
	void OnEffectAssetLoaded(FPrimaryAssetId LoadedAssetId);
	void CheckAsyncLoadComplete();

	FStreamableManager StreamableManager;

	uint8 EffectDataCount = 1;

	FOnCompleteLoad_A OnCompleteLoad_A;
	FOnCompleteLoad_B OnCompleteLoad_B;
	FOnCompleteLoad_C OnCompleteLoad_C;
	FOnCompleteLoad_D OnCompleteLoad_D;

	bool bCompleteA = false;
	bool bCompleteB = false;
	bool bCompleteC = false;
	bool bCompleteD = false;
};