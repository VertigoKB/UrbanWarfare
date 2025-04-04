// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareController.h"

#include "UrbanWarfare/Frameworks/Components/CinematicComponent.h"
#include "UrbanWarfare/Frameworks/Components/PlayerSpawnerComponent.h"
//#include "UrbanWarfare/Frameworks/Components/RequestSpawnComponent.h"
#include "UrbanWarfare/Frameworks/WarfareHud.h"
#include "UrbanWarfare/Frameworks/WarfareGameMode.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

AWarfareController::AWarfareController()
{
}

UBlueprintConfig* AWarfareController::GetBlueprintConfig() { return BlueprintConfig; }
APlayerBase* AWarfareController::GetPlayerPawn() { return PlayerPawn; }
void AWarfareController::SetPlayerPawn(APlayerBase* InPlayerPawn) { PlayerPawn = InPlayerPawn; }

void AWarfareController::SelectTeam(ETeam InTeam)
{
	if (HasAuthority())
	{
		ClientRequestStopSequenceToHud();
		GetWorld()->GetAuthGameMode<AWarfareGameMode>()->GetPlayerSpawnerComponent()->RequestSpawnPlayer(InTeam, this);
	}
	else
		ServerSelectTeam(InTeam);

}

void AWarfareController::ServerSelectTeam_Implementation(ETeam InTeam)
{
	SelectTeam(InTeam);
}

void AWarfareController::SpawnPlayer(ETeam InTeam, const FTransform& SpawnTrasform)
{
	TSubclassOf<APlayerBase> PlayerType = nullptr;

	if (!BlueprintConfig)
		return;

	switch (InTeam)
	{
	case ETeam::Terrorist:
		PlayerType = BlueprintConfig->Terrorist;
		break;
	case ETeam::CounterTrist:
		PlayerType = BlueprintConfig->CounterTrist;
		break;
	}

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	PlayerPawn = GetWorld()->SpawnActor<APlayerBase>(PlayerType, SpawnTrasform, SpawnParam);

	Possess(PlayerPawn);
}

void AWarfareController::ClientRequestStopSequenceToHud_Implementation()
{
	GetHUD<AWarfareHud>()->GetCinematicComponent()->StopAllSequence();
}

void AWarfareController::BeginPlay()
{
	Super::BeginPlay();

	if (!BlueprintConfig)
	{
		LOG_NULL(BlueprintConfig);
	}

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}

void AWarfareController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AWarfareController::OnUnPossess()
{
	Super::OnUnPossess();
}
