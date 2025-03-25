// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareController.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/PlayerBehaviorComponent.h"
#include "UrbanWarfare/Frameworks/WarfareGameMode.h"
#include "UrbanWarfare/Frameworks/ControllerComponents/UserInterfaceComponent.h"
#include "UrbanWarfare/UI/MainMenu.h"

AWarfareController::AWarfareController()
{
	//APlayerCameraManager* temp;
	//temp->ViewPitchMin
	UserInterfaceComponent = CreateDefaultSubobject<UUserInterfaceComponent>(TEXT("UserInterfaceComponent"));
}

UUserInterfaceComponent* AWarfareController::GetUserInterfaceComponent() { return UserInterfaceComponent; }

//APlayerBase* AWarfareController::GeWarfarePawn() { return ; }

void AWarfareController::BeginPlay()
{
	Super::BeginPlay();

}

void AWarfareController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AWarfareController::OnUnPossess()
{
	Super::OnUnPossess();
}
