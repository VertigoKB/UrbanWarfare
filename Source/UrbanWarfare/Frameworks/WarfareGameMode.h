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

	class UPlayerSpawnerComponent* GetPlayerSpawnerComponent();

public:
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, 
		const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

protected:
	void AdjustTextureLevel();

protected:

	UPROPERTY()
	TObjectPtr<class UPlayerSpawnerComponent> PlayerSpawnerComponent;

};
