// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamScoreHud.h"
#include "UrbanWarfare/UI/MainWidget.h"
#include "UrbanWarfare/Frameworks/WarfareGameState.h"

void UTeamScoreHud::ExternalInitialize(UMainWidget* const InOwner, UWorld* const InWorld)
{
	World = InWorld;
	MainWidget = InOwner;

	WarfareGameState = World->GetGameState<AWarfareGameState>();
	WarfareGameState->OnGoalScoreChange.BindUObject(this, &UTeamScoreHud::SetGoalScore);
	WarfareGameState->OnTerroristScoreChange.BindUObject(this, &UTeamScoreHud::SetTerroristScore);
	WarfareGameState->OnCounterTristScoreChange.BindUObject(this, &UTeamScoreHud::SetCounterTristScore);

	SetGoalScore(WarfareGameState->GetCurrentGoalScore());
	SetTerroristScore(WarfareGameState->GetCurrentTerroristScore());
	SetCounterTristScore(WarfareGameState->GetCurrentCounterTristScore());
}

void UTeamScoreHud::SetGoalScore(uint16 InScore)
{
	FText Content = FText::AsNumber(InScore);
	MainWidget->SetTextBlockContent(EMainWidgetElem::GoalScore, Content);
}

void UTeamScoreHud::SetCounterTristScore(uint16 InScore)
{
	FText Content = FText::AsNumber(InScore);
	MainWidget->SetTextBlockContent(EMainWidgetElem::CounterTristScore, Content);
}

void UTeamScoreHud::SetTerroristScore(uint16 InScore)
{
	FText Content = FText::AsNumber(InScore);
	MainWidget->SetTextBlockContent(EMainWidgetElem::TerroristScore, Content);
}
