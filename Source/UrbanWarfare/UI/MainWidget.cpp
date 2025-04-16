// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Image.h"

#include "MainWidgetElements/WeaponHud.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponHud = NewObject<UWeaponHud>();
	WeaponHud->ElementInitialize(ImageRifle, ImagePistol, GetWorld());

	GetOwningPlayer<AWarfareController>()->OnPlayerSpawned.BindUObject(this, &UMainWidget::OnPlayerSpawned);

	SetVisibility(ESlateVisibility::Hidden);

	//GetWorld()->GetTimerManager().SetTimer(InitHandle, FTimerDelegate::CreateLambda([this]() {

	//	if (InitCount > 0)
	//	{
	//		if (!InitConstruct())
	//		{
	//			LOG_EFUNC(TEXT("Failed to initialize MainWidget. Count: %d"), --InitCount);
	//		}
	//		else
	//		{
	//			LOG_EFUNC(TEXT("MainWidget initialization complete."));
	//			GetWorld()->GetTimerManager().ClearTimer(InitHandle);
	//		}
	//	}
	//	else
	//	{
	//		LOG_EFUNC(TEXT("Failed to initialize MainWidget."));
	//		GetWorld()->GetTimerManager().ClearTimer(InitHandle);
	//	}

	//	

	//	}), 0.1f, true);
	
}

void UMainWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

bool UMainWidget::InitConstruct()
{
	switch (true)
	{
	case true:
	default:
		AWarfareController* WarfareController = GetOwningPlayer<AWarfareController>();
		if (!WarfareController)
		{
			LOG_EFUNC(TEXT("WarfareController is null."));
			break;
		}

		APlayerBase* OwnerPlayer = WarfareController->GetPlayerPawn();
		if (!OwnerPlayer)
		{
			LOG_EFUNC(TEXT("PlayerPawn is null."));
			break;
		}
		
		WeaponComponent = OwnerPlayer->GetWeaponComponent();
		if (!WeaponComponent)
		{
			LOG_EFUNC(TEXT("WeaponComponent is null."));
			break;
		}

		WeaponComponent->OnInventoryChange.BindUObject(WeaponHud, &UWeaponHud::OnInventoryChange);
		LOG_EFUNC(TEXT("Mainwidget initialize complete."));

		return true;
	}

	return false;
}

void UMainWidget::OnPlayerSpawned(APlayerBase* InPlayer)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	WeaponComponent = InPlayer->GetWeaponComponent();
	WeaponComponent->OnInventoryChange.BindUObject(WeaponHud, &UWeaponHud::OnInventoryChange);
	WeaponComponent->OnLocalPlayerEquipWeapon.BindUObject(WeaponHud, &UWeaponHud::OnEquipWeapon);
}
 