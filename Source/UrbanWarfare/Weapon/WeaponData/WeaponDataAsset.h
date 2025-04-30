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

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UTexture2D> WeaponIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<class UParticleSystem> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<USoundBase> SingleGunShotSound;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly)
	EFiringMode FiringMode;

	UPROPERTY(EditDefaultsOnly)
	uint16 LoadableAmmoPerMag;

	UPROPERTY(EditDefaultsOnly)
	uint16 MaxAmmo;

	UPROPERTY(EditDefaultsOnly)
	float RoundPerMinute;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "좌우 반동 최대값"))
	float HorizontalRecoil = 0.f;
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "상하 반동 최대값"))
	float VerticalRecoil = 0.f;


	float RoundInterval;
	//FQuantizedFloat CompressedRPM;
	//FQuantizedFloat CompressedDamage;

};
