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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class UBlueprintConfig* GetBlueprintConfig();
	class APlayerBase* GetPlayerPawn();
	void SetPlayerPawn(class APlayerBase* InPlayerPawn);

	void SelectTeam(ETeam InTeam);

	UFUNCTION(Server, Reliable)
	void ServerSelectTeam(ETeam InTeam);

	void SpawnPlayer(ETeam InTeam, const FTransform& SpawnTrasform);

	UFUNCTION(Client, Reliable)
	void ClientRequestStopSequenceToHud();

	inline bool IsListenServerHost() const { return bIsListenHost; }
	inline ETeam GetMyTeam() const { return MyTeam; }
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
	void OnRep_PlayerPawn();

	UFUNCTION()
	void OnRep_bLetClientShowMouns();

public:
	FOnPlayerSpawned OnPlayerSpawned;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APlayerBase> CounterTrist;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APlayerBase> Terrorist;

	UPROPERTY(ReplicatedUsing = OnRep_bLetClientShowMouns)
	bool bLetClientShowMounse = true;

private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerPawn)
	TObjectPtr<class APlayerBase> PlayerPawn;

	bool bIsListenHost = false;

	UPROPERTY(Replicated)
	ETeam MyTeam = ETeam::None;
};