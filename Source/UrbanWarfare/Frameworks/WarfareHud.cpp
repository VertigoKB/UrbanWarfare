// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareHud.h"
#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "UrbanWarfare/UI/MainMenu.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

AWarfareHud::AWarfareHud()
{
}

void AWarfareHud::BeginPlay()
{
	Super::BeginPlay();

	CreateMainMenuWidget();

}

void AWarfareHud::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

UMainMenu* AWarfareHud::GetWidgetMainMenu() { return MainMenu; }

void AWarfareHud::CreateMainMenuWidget()
{
	if (!BlueprintConfig)
	{
		LOG_NULL(BlueprintConfig);
		return;
	}

	MainMenu = CreateWidget<UMainMenu>(GetWorld(), BlueprintConfig->MainMenu);
	if (MainMenu)
		MainMenu->AddToViewport();
}
