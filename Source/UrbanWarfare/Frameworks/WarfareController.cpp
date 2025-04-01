// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareController.h"

#include "UrbanWarfare/Frameworks/Components/UserInterfaceComponent.h"
#include "UrbanWarfare/Frameworks/Components/CinematicComponent.h"
#include "UrbanWarfare/Frameworks/WarfareHud.h"

AWarfareController::AWarfareController()
{
	UserInterfaceComponent = CreateDefaultSubobject<UUserInterfaceComponent>(TEXT("UserInterfaceComponent"));
}

UUserInterfaceComponent* AWarfareController::GetUserInterfaceComponent() { return UserInterfaceComponent; }

void AWarfareController::RequestStopSequenceToHud()
{
	//if (!HasAuthority() || 
	//	(GetWorld()->GetNetMode() == NM_ListenServer))

	GetHUD<AWarfareHud>()->GetCinematicComponent()->StopAllSequence();
}

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
