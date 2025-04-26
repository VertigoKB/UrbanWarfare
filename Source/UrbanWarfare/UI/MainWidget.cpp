// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

#include "MainWidgetElements/WeaponHud.h"
#include "MainWidgetElements/AmmoHud.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

void UMainWidget::SetWidgetVisibility(EMainWidgetElem TargetWidget, ESlateVisibility InVisibility)
{
	if (UWidget** FoundWidget = WidgetsMap.Find(TargetWidget))
	{
		if (*FoundWidget)
			(*FoundWidget)->SetVisibility(InVisibility);
		else
		{
			LOG_EFUNC(TEXT("요청된 위젯의 데이터가 없음."));
		}
	}
}

void UMainWidget::SetTextBlockContent(EMainWidgetElem TargetWidget, FText InContent)
{
	if (UWidget** FoundWidget = WidgetsMap.Find(TargetWidget))
	{
		UTextBlock* TargetTextComp = Cast<UTextBlock>(*FoundWidget);
		if (TargetTextComp)
			TargetTextComp->SetText(InContent);
		else
		{
			LOG_EFUNC(TEXT("텍스트 블록을 찾는데 실패함."));
		}
	}
	else
	{
		LOG_EFUNC(TEXT("요청된 위젯의 데이터가 없음."));
	}
}

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponHud = NewObject<UWeaponHud>();
	WeaponHud->ExternalInitialize(ImageRifle, ImagePistol, GetWorld());

	

	GetOwningPlayer<AWarfareController>()->OnPlayerSpawned.BindUObject(this, &UMainWidget::OnPlayerSpawned);

	SetVisibility(ESlateVisibility::Hidden);

	WidgetsMap.Add(EMainWidgetElem::ImagePistol, ImagePistol);
	WidgetsMap.Add(EMainWidgetElem::ImageRifle, ImageRifle);
	WidgetsMap.Add(EMainWidgetElem::AmmoCondition, AmmoCondition);
	WidgetsMap.Add(EMainWidgetElem::RemainAmmo, RemainAmmo);
	WidgetsMap.Add(EMainWidgetElem::ExtraAmmo, ExtraAmmo);


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

//bool UMainWidget::InitConstruct()
//{
//	switch (true)
//	{
//	case true:
//	default:
//		AWarfareController* WarfareController = GetOwningPlayer<AWarfareController>();
//		if (!WarfareController)
//		{
//			LOG_EFUNC(TEXT("WarfareController is null."));
//			break;
//		}
//
//		APlayerBase* OwnerPlayer = WarfareController->GetPlayerPawn();
//		if (!OwnerPlayer)
//		{
//			LOG_EFUNC(TEXT("PlayerPawn is null."));
//			break;
//		}
//		
//		WeaponComponent = OwnerPlayer->GetWeaponComponent();
//		if (!WeaponComponent)
//		{
//			LOG_EFUNC(TEXT("WeaponComponent is null."));
//			break;
//		}
//
//		WeaponComponent->OnInventoryChange.BindUObject(WeaponHud, &UWeaponHud::OnInventoryChange);
//		LOG_EFUNC(TEXT("Mainwidget initialize complete."));
//
//		return true;
//	}
//
//	return false;
//}

void UMainWidget::OnPlayerSpawned(APlayerBase* InPlayer)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);

	WeaponComponent = InPlayer->GetWeaponComponent();
	WeaponComponent->OnInventoryChange.BindUObject(WeaponHud, &UWeaponHud::OnInventoryChange);
	WeaponComponent->OnLocalPlayerEquipWeapon.BindUObject(WeaponHud, &UWeaponHud::OnEquipWeapon);

	AmmoHud = NewObject<UAmmoHud>();
	AmmoHud->ExternalInitialize(this, GetWorld(), WeaponComponent);
}
 