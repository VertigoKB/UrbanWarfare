// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoHandler.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/CombatComponent.h"

void UAmmoHandler::ExternalInitialize(APlayerBase* const InRootOwner, UWeaponComponent* const InOwnerComp)
{
	OwnerPawn = InRootOwner;
	OwnerPawn->OnspecifiedMultiplayCase.AddUObject(this, &UAmmoHandler::OnSpecifiedMultiplayCase);

	World = OwnerPawn->GetWorld();
	WeaponComponent = InOwnerComp;
	CombatComponent = OwnerPawn->GetCombatComponent();
	//CombatComponent->OnAttack.AddUObject(this, &UAmmoHandler::Client_Shoot);
}

void UAmmoHandler::RefreshAmmoHandler(const FWeaponAmmoData& InData, const EWeaponType InType)
{
	if (!bIsLocal)
		return;

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
	if (!bIsLocal)
		return;

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

void UAmmoHandler::OnSpecifiedMultiplayCase(EMultiplayCase InMultiplayCase)
{
	OwnerMultiplayCase = InMultiplayCase;

	if (OwnerMultiplayCase == EMultiplayCase::Server_ListenHost ||
		OwnerMultiplayCase == EMultiplayCase::Client_LocalPlayer)
		bIsLocal = true;

}

bool UAmmoHandler::IsAmmoRemaInInMag()
{
	bool bFlag = false;

	if (CurrentAmmoInMag > 0)
		bFlag = true;

	//LOG_NETEFUNC_UOBJECT(World, TEXT("CurrentAmmoInMag: %d"), CurrentAmmoInMag);

	bool CombatCompFlag = CombatComponent->GetbIsNoAmmoInMag();

	if (CombatCompFlag != bFlag)
		CombatComponent->Server_SetbIsAmmoInMag(bFlag);

	return bFlag;
}