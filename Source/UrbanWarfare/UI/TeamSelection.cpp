// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamSelection.h"
#include "Components/Button.h"

#include "UrbanWarfare/Frameworks/WarfareController.h"

void UTeamSelection::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonCounterTrist->OnClicked.AddDynamic(this, &UTeamSelection::OnClickedBlueTeam);
	ButtonTerrorist->OnClicked.AddDynamic(this, &UTeamSelection::OnClickedRedTeam);

}

void UTeamSelection::OnClickedRedTeam()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer<AWarfareController>()->SetInputMode(InputMode);
	GetOwningPlayer<AWarfareController>()->SelectTeam(ETeam::Terrorist);
	RemoveFromParent();
}

void UTeamSelection::OnClickedBlueTeam()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer<AWarfareController>()->SetInputMode(InputMode);
	GetOwningPlayer<AWarfareController>()->SelectTeam(ETeam::CounterTrist);
	RemoveFromParent();
}
