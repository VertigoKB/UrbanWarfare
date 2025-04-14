// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareHud.h"
#include "Components/Button.h"

#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Frameworks/Components/CinematicComponent.h"
#include "UrbanWarfare/UI/TeamSelection.h"
#include "UrbanWarfare/UI/MainWidget.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

AWarfareHud::AWarfareHud()
{
	CinematicComponent = CreateDefaultSubobject<UCinematicComponent>(TEXT("CinematicComponent"));
}

UCinematicComponent* AWarfareHud::GetCinematicComponent() { return CinematicComponent; }
UTeamSelection* AWarfareHud::GetTeamSelectionWidget() { return TeamSelection; }

void AWarfareHud::BeginPlay()
{
	Super::BeginPlay();

	if (!BlueprintConfig)
		return;

	CreateTeamSelectionWidget();
	CreateMainWidget();

	//AWarfareController* WarfareController = Cast<AWarfareController>(GetWorld()->GetFirstPlayerController());

}

void AWarfareHud::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


void AWarfareHud::CreateTeamSelectionWidget()
{
	TeamSelection = CreateWidget<UTeamSelection>(GetWorld(), BlueprintConfig->TeamSelection);
	if (TeamSelection)
		TeamSelection->AddToViewport();
}

void AWarfareHud::CreateMainWidget()
{
	MainWidget = CreateWidget<UMainWidget>(GetWorld(), BlueprintConfig->MainWidget);
	if (MainWidget)
		MainWidget->AddToViewport();
}
