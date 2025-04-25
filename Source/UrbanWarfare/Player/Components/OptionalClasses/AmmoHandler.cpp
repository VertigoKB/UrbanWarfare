// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoHandler.h"
#include "UrbanWarfare/Player/PlayerBase.h"

void UAmmoHandler::ExternalInitialize(APlayerBase* const InRootOwner, UWeaponComponent* const InOwnerComp)
{
	OwnerPawn = InRootOwner;
	World = OwnerPawn->GetWorld();
	WeaponComponent = InOwnerComp;
}

void UAmmoHandler::RefreshAmmoData(const FWeaponAmmoData& InData)
{
	CurrentAmmoInMag = InData.AmmoInMag;
	CurrentExtraAmmo = InData.ExtraAmmo;
}

void UAmmoHandler::RefreshCurrentWeaponType(const EWeaponType InType)
{
	CurrentWeaponType = InType;

	if (InType == EWeaponType::UnArmed)
		OnEmptyHand.ExecuteIfBound();
}

void UAmmoHandler::Client_Shoot()
{
	if (!bIsNoAmmo)
	{
		CurrentAmmoInMag--;
		if (CurrentAmmoInMag == 0)
			bIsNoAmmo = true;
	}
	else
		OnShotButNoAmmo.ExecuteIfBound();
}
