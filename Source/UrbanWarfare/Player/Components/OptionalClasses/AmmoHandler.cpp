// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoHandler.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/CombatComponent.h"

void UAmmoHandler::ExternalInitialize(APlayerBase* const InRootOwner, UWeaponComponent* const InOwnerComp)
{
	OwnerPawn = InRootOwner;
	World = OwnerPawn->GetWorld();
	WeaponComponent = InOwnerComp;
	CombatComponent = OwnerPawn->GetCombatComponent();
	CombatComponent->OnAttack.AddUObject(this, &UAmmoHandler::Client_Shoot);
}

void UAmmoHandler::RefreshAmmoHandler(const FWeaponAmmoData& InData, const EWeaponType InType)
{
	CurrentAmmoInMag = InData.AmmoInMag;
	CurrentExtraAmmo = InData.ExtraAmmo;

	CurrentWeaponType = InType;

	IsAmmoRemaInInMag();

	if (InType == EWeaponType::UnArmed)
		OnEmptyHand.ExecuteIfBound(false);
	else
	{
		OnEmptyHand.ExecuteIfBound(true);
		OnUpdateAmmoInMag.ExecuteIfBound(CurrentAmmoInMag);
		OnUpdateExtraAmmo.ExecuteIfBound(CurrentExtraAmmo);
	}
}

void UAmmoHandler::Client_Shoot()
{
	if (IsAmmoRemaInInMag())
	{
		OnUpdateAmmoInMag.ExecuteIfBound(--CurrentAmmoInMag);
		IsAmmoRemaInInMag();
	}
}

FWeaponAmmoData UAmmoHandler::GetAmmoDAta() const
{
	FWeaponAmmoData Data;
	Data.AmmoInMag = CurrentAmmoInMag;
	Data.ExtraAmmo = CurrentExtraAmmo;

	return Data;
}

bool UAmmoHandler::IsAmmoRemaInInMag()
{
	bool bFlag = false;

	if (CurrentAmmoInMag > 0)
		bFlag = true;

	bool CombatCompFlag = CombatComponent->GetbIsNoAmmoInMag();

	if (CombatCompFlag != bFlag)
		CombatComponent->Server_SetbIsAmmoInMag(bFlag);

	return bFlag;
}