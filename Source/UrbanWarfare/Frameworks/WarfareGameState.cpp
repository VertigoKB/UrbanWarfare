// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareGameState.h"
#include "Net/UnrealNetwork.h"

void AWarfareGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWarfareGameState, GoalScore);
	DOREPLIFETIME(AWarfareGameState, TerroristScore);
	DOREPLIFETIME(AWarfareGameState, CounterTristScore);

}

void AWarfareGameState::Server_AddTerroristScore()
{
	TerroristScore++;
	OnRep_TerroristScore();
}

void AWarfareGameState::Server_AddCounterTristScore()
{
	CounterTristScore++;
	OnRep_CounterTristScore();
}

void AWarfareGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		OnRep_GoalSocre();
}

void AWarfareGameState::OnRep_GoalSocre()
{
	OnGoalScoreChange.ExecuteIfBound(GoalScore);
}

void AWarfareGameState::OnRep_TerroristScore()
{
	OnTerroristScoreChange.ExecuteIfBound(TerroristScore);
}

void AWarfareGameState::OnRep_CounterTristScore()
{
	OnCounterTristScoreChange.ExecuteIfBound(CounterTristScore);
}
