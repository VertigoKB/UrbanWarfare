// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SerializedWeaponInventory.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "UrbanWarfare/Common/CommonStructs.h"
#include "AmmoHandler.generated.h"

DECLARE_DELEGATE(FOnShotButNoAmmo)
DECLARE_DELEGATE(FOnEmptyHand)

UCLASS()
class URBANWARFARE_API UAmmoHandler : public UObject
{
	GENERATED_BODY()
	
public:
	void ExternalInitialize(class APlayerBase* const InRootOwner, class UWeaponComponent* const InOwnerComp);

	void RefreshAmmoData(const FWeaponAmmoData& InData);
	void RefreshCurrentWeaponType(const EWeaponType InType);

	void Client_Shoot();

public:
	FOnShotButNoAmmo OnShotButNoAmmo;
	FOnEmptyHand OnEmptyHand;

private:
	uint16 CurrentAmmoInMag = 0;
	uint16 CurrentExtraAmmo = 0;

	bool bIsNoAmmo = true;

	EWeaponType CurrentWeaponType = EWeaponType::UnArmed;

private:
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	UPROPERTY()
	TObjectPtr<UWorld> World;
	UPROPERTY()
	TArray<FInventoryItem> CopiedInventory;
};
