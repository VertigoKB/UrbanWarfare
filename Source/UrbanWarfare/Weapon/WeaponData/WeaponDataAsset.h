// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "UrbanWarfare/Common/QuantizedFloat.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class URBANWARFARE_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Weapon", GetFName());
	}

	void InitiallizeValues();

	UPROPERTY(EditDefaultsOnly)
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly)
	uint8 WeaponIdNumber;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> WeaponIcon;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UParticleSystem> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly)
	float RoundPerMinute;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)

	float RoundInterval;

	UPROPERTY(EditDefaultsOnly)
	uint8 MaxAmmoPerMag;
	//FQuantizedFloat CompressedRPM;
	//FQuantizedFloat CompressedDamage;

};
