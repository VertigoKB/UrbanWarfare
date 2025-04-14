// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "WarfareController.generated.h"

DECLARE_DELEGATE_OneParam(FOnPlayerSpawned, class APlayerBase*)

UCLASS()
class URBANWARFARE_API AWarfareController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWarfareController();

	class UBlueprintConfig* GetBlueprintConfig();
	class APlayerBase* GetPlayerPawn();
	void SetPlayerPawn(class APlayerBase* InPlayerPawn);

	void SelectTeam(ETeam InTeam);

	UFUNCTION(Server, Reliable)
	void ServerSelectTeam(ETeam InTeam);

	void SpawnPlayer(ETeam InTeam, const FTransform& SpawnTrasform);

	UFUNCTION(Client, Reliable)
	void ClientRequestStopSequenceToHud();
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	FOnPlayerSpawned OnPlayerSpawned;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBlueprintConfig> BlueprintConfig;

private:
	UPROPERTY()
	TObjectPtr<class APlayerBase> PlayerPawn;
};