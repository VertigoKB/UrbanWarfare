// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WarfarePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API AWarfarePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(Server, Reliable)
	void OnClickedGameStart();

public:
	//UPROPERTY(Replicated)
	//bool bClickedGameStart;

	//EPlayerGameCondition PlayerGameCondition = EPlayerGameCondition::MainMenu;
};
