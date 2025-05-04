// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WarfareGameState.generated.h"

DECLARE_DELEGATE_OneParam(FOnGoalScoreChange, uint16)
DECLARE_DELEGATE_OneParam(FOnTerroristScoreChange, uint16)
DECLARE_DELEGATE_OneParam(FOnCounterTristScoreChange, uint16)

UCLASS()
class URBANWARFARE_API AWarfareGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Server_AddTerroristScore();
	void Server_AddCounterTristScore();

	inline uint16 GetCurrentGoalScore() const { return GoalScore; }
	inline uint16 GetCurrentTerroristScore() const { return TerroristScore; }
	inline uint16 GetCurrentCounterTristScore() const { return CounterTristScore; }
protected:
	virtual void BeginPlay() override;

public:
	FOnGoalScoreChange OnGoalScoreChange;
	FOnTerroristScoreChange OnTerroristScoreChange;
	FOnCounterTristScoreChange OnCounterTristScoreChange;

private:
	UFUNCTION()
	void OnRep_GoalSocre();
	UFUNCTION()
	void OnRep_TerroristScore();
	UFUNCTION()
	void OnRep_CounterTristScore();
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_GoalSocre)
	uint16 GoalScore = 30;
	UPROPERTY(ReplicatedUsing = OnRep_TerroristScore)
	uint16 TerroristScore = 0;
	UPROPERTY(ReplicatedUsing = OnRep_CounterTristScore)
	uint16 CounterTristScore = 0;
};
