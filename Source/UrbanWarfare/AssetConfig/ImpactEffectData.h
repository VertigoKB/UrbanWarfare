// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ImpactEffectData.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UImpactEffectData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ImpactEffect", GetFName());
	}

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> SoundImapctBody;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> SoundImpactEnv;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> ParticleImpactBody;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> ParticleImpactEnv;

};
