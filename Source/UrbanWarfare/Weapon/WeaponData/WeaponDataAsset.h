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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 WeaponIdNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTexture2D> WeaponIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UParticleSystem> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RoundPerMinute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage;

	float RoundInterval;
	//FQuantizedFloat CompressedRPM;
	//FQuantizedFloat CompressedDamage;

};
