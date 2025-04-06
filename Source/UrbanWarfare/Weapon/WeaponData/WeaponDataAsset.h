// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UrbanWarfare/Common/CommonEnums.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage;

};
