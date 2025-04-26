// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoHud.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/UI/MainWidget.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Player/Components/OptionalClasses/AmmoHandler.h"

void UAmmoHud::ExternalInitialize(UUserWidget* const InOwner, UWorld* const InWorld, UWeaponComponent* const InWeaponComp)
{
	MainWidget = Cast<UMainWidget>(InOwner);
	World = InWorld;

	AmmoHandler = InWeaponComp->GetAmmoHandler();
	if (AmmoHandler)
		OnSuccessfullyInitialize();

	MainWidget->SetWidgetVisibility(EMainWidgetElem::AmmoCondition, ESlateVisibility::Hidden);
}

void UAmmoHud::OnSuccessfullyInitialize()
{
	LOG_EFUNC(TEXT("Initialization successfully complete."));
	AmmoHandler->OnUpdateAmmoInMag.BindUObject(this, &UAmmoHud::UpdateAmmoInMagText);
	AmmoHandler->OnUpdateExtraAmmo.BindUObject(this, &UAmmoHud::UpdateExtraAmmoText);
	AmmoHandler->OnEmptyHand.BindUObject(this, &UAmmoHud::OnEmptyHand);
}

void UAmmoHud::BeginDestroy()
{
	Super::BeginDestroy();

	if (World)
		World->GetTimerManager().ClearAllTimersForObject(this);
}

void UAmmoHud::OnEmptyHand(bool bIsVisible)
{
	if (bIsVisible)
		MainWidget->SetWidgetVisibility(EMainWidgetElem::AmmoCondition, ESlateVisibility::HitTestInvisible);
	else
		MainWidget->SetWidgetVisibility(EMainWidgetElem::AmmoCondition, ESlateVisibility::Hidden);
}

void UAmmoHud::UpdateAmmoInMagText(uint16 InCurrentAmmo)
{
	FText Contents = FText::AsNumber(InCurrentAmmo);
	MainWidget->SetTextBlockContent(EMainWidgetElem::RemainAmmo, Contents);
}

void UAmmoHud::UpdateExtraAmmoText(uint16 InExtraAmmo)
{
	FText Contents = FText::AsNumber(InExtraAmmo);
	MainWidget->SetTextBlockContent(EMainWidgetElem::ExtraAmmo, Contents);
}
