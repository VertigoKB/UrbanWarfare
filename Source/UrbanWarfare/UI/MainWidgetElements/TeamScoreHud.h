// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TeamScoreHud.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UTeamScoreHud : public UObject
{
	GENERATED_BODY()
	
public:
	void ExternalInitialize(class UMainWidget* const InOwner, UWorld* const InWorld);

	void SetGoalScore(uint16 InScore);
	void SetCounterTristScore(uint16 InScore);
	void SetTerroristScore(uint16 InScore);
private:
	UWorld* World;
	
	UPROPERTY()
	TObjectPtr<UMainWidget> MainWidget;
	UPROPERTY()
	TObjectPtr<class AWarfareGameState> WarfareGameState;
};
