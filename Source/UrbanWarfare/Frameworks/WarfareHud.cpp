// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareHud.h"
#include "Components/Button.h"

#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Frameworks/Components/UserInterfaceComponent.h"
#include "UrbanWarfare/Frameworks/Components/CinematicComponent.h"
#include "UrbanWarfare/UI/MainMenu.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

AWarfareHud::AWarfareHud()
{
	CinematicComponent = CreateDefaultSubobject<UCinematicComponent>(TEXT("CinematicComponent"));
}

UCinematicComponent* AWarfareHud::GetCinematicComponent() { return CinematicComponent; }
UMainMenu* AWarfareHud::GetWidgetMainMenu() { return MainMenu; }

void AWarfareHud::BeginPlay()
{
	Super::BeginPlay();

	CreateMainMenuWidget();

	AWarfareController* WarfareController = Cast<AWarfareController>(GetWorld()->GetFirstPlayerController());
	UUserInterfaceComponent* UserInterfaceComponent = WarfareController->GetUserInterfaceComponent();
	MainMenu->ButtonGameStart->OnReleased.AddDynamic(UserInterfaceComponent, &UUserInterfaceComponent::OnReleaseGameStart);

}

void AWarfareHud::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


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
