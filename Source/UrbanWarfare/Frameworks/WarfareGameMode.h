// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarfareGameMode.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API AWarfareGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AWarfareGameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TArray<AActor*> PlayerStartFinder;

	UPROPERTY()
	TArray<AActor*> CounterTristStart;

	UPROPERTY()
	TArray<AActor*> TerroristStart;
};
