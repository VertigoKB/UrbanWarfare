// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoHandler.h"
#include "UrbanWarfare/Player/PlayerBase.h"

void UAmmoHandler::ExternalInitialize(APlayerBase* const InRootOwner, UWeaponComponent* const InOwnerComp)
{
	OwnerPawn = InRootOwner;
	World = OwnerPawn->GetWorld();
	WeaponComponent = InOwnerComp;
}

void UAmmoHandler::RefreshAmmoHandler(const FWeaponAmmoData& InData, const EWeaponType InType)
{
	CurrentAmmoInMag = InData.AmmoInMag;
	CurrentExtraAmmo = InData.ExtraAmmo;

	CurrentWeaponType = InType;

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
	if (!bIsNoAmmo)
	{
		OnUpdateAmmoInMag.ExecuteIfBound(--CurrentAmmoInMag);
		if (CurrentAmmoInMag == 0)
			bIsNoAmmo = true;
	}
	else
		OnShotButNoAmmo.ExecuteIfBound();
}

FWeaponAmmoData UAmmoHandler::GetAmmoDAta() const
{
	FWeaponAmmoData Data;
	Data.AmmoInMag = CurrentAmmoInMag;
	Data.ExtraAmmo = CurrentExtraAmmo;

	return Data;
}