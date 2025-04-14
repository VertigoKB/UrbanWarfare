// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHud.h"
#include "Components/Image.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"

#include "UrbanWarfare/UI/MainWidget.h"

void UWeaponHud::ElementInitialize(UImage* InImageRifle, UImage* InImagePistol, UWorld* InWorld)
{
	ImageRifle = InImageRifle;
	ImagePistol = InImagePistol;
	World = InWorld;

	WeaponPreLoader = World->GetGameInstance()->GetSubsystem<UWeaponPreLoader>();
	ImageRifle->SetVisibility(ESlateVisibility::Hidden);
	ImagePistol->SetVisibility(ESlateVisibility::Hidden);

}

void UWeaponHud::OnInventoryChange(const TArray<FInventoryItem>& InInventory)
{
	uint8 InventoryRifle = InInventory[static_cast<uint8>(EWeaponType::Rifle)].WeaponId;
	uint8 InventoryPistol = InInventory[static_cast<uint8>(EWeaponType::Pistol)].WeaponId;

	if (InventoryRifle != 0)
	{
		UWeaponDataAsset* RifleData = WeaponPreLoader->GetWeaponDataByWeaponId(InventoryRifle);
		ImageRifle->SetBrushFromTexture(RifleData->WeaponIcon);
		ImageRifle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
		ImageRifle->SetVisibility(ESlateVisibility::Hidden);

	if (InventoryPistol != 0)
	{
		UWeaponDataAsset* PistolData = WeaponPreLoader->GetWeaponDataByWeaponId(InventoryPistol);
		ImagePistol->SetBrushFromTexture(PistolData->WeaponIcon);
		ImagePistol->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
		ImagePistol->SetVisibility(ESlateVisibility::Hidden);

}

void UWeaponHud::OnEquipWeapon(EWeaponType InType)
{
	switch (InType)
	{
	case EWeaponType::UnArmed:
		break;
	case EWeaponType::Rifle:
		ImageRifle->SetOpacity(1.f);
		ImagePistol->SetOpacity(0.5f);
		break;
	case EWeaponType::Pistol:
		ImageRifle->SetOpacity(0.5f);
		ImagePistol->SetOpacity(1.f);
		break;
	}
}

void UWeaponHud::BeginDestroy()
{
	Super::BeginDestroy();
}
