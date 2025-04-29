// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SerializedWeaponInventory.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "UrbanWarfare/Common/CommonStructs.h"
#include "AmmoHandler.generated.h"

DECLARE_DELEGATE_OneParam(FOnEmptyHand, bool)
DECLARE_DELEGATE_OneParam(FOnUpdateAmmoInMag, uint16)
DECLARE_DELEGATE_OneParam(FOnUpdateExtraAmmo, uint16)

UCLASS()
class URBANWARFARE_API UAmmoHandler : public UObject
{
	GENERATED_BODY()
	
public:
	void ExternalInitialize(class APlayerBase* const InRootOwner, class UWeaponComponent* const InOwnerComp);

	void RefreshAmmoHandler(const FWeaponAmmoData& InData, const EWeaponType InType);

	void Client_Shoot();

	FWeaponAmmoData GetAmmoData() const;
	void SetAmmoData(const FWeaponAmmoData InAmmoData);

private:
	void OnSpecifiedMultiplayCase(EMultiplayCase InMultiplayCase);
	bool IsAmmoRemaInInMag();

public:
	FOnEmptyHand OnEmptyHand;
	FOnUpdateAmmoInMag OnUpdateAmmoInMag;
	FOnUpdateExtraAmmo OnUpdateExtraAmmo;


private:
	uint16 CurrentAmmoInMag = 0;
	uint16 CurrentExtraAmmo = 0;

	EWeaponType CurrentWeaponType = EWeaponType::UnArmed;

	bool bIsLocal = false;
	EMultiplayCase OwnerMultiplayCase = EMultiplayCase::Unspecified;
private:
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	UPROPERTY()
	TObjectPtr<class UCombatComponent> CombatComponent;
	UPROPERTY()
	TObjectPtr<UWorld> World;
	UPROPERTY()
	TArray<FInventoryItem> CopiedInventory;
};
