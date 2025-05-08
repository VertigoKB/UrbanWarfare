// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareController.h"

#include "UrbanWarfare/Frameworks/Components/CinematicComponent.h"
#include "UrbanWarfare/Frameworks/Components/PlayerSpawnerComponent.h"
//#include "UrbanWarfare/Frameworks/Components/RequestSpawnComponent.h"
#include "UrbanWarfare/Frameworks/WarfareHud.h"
#include "UrbanWarfare/Frameworks/WarfareGameMode.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

AWarfareController::AWarfareController()
{
}

void AWarfareController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWarfareController, PlayerPawn);
	DOREPLIFETIME(AWarfareController, MyTeam);
	DOREPLIFETIME(AWarfareController, bLetClientShowMounse);
}

APlayerBase* AWarfareController::GetPlayerPawn() { return PlayerPawn; }
void AWarfareController::SetPlayerPawn(APlayerBase* InPlayerPawn) { PlayerPawn = InPlayerPawn; }

void AWarfareController::SelectTeam(ETeam InTeam)
{
	if (HasAuthority())
	{
		ClientRequestStopSequenceToHud();
		MyTeam = InTeam;
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

	switch (InTeam)
	{
	case ETeam::Terrorist:
		PlayerType = Terrorist;
		break;
	case ETeam::CounterTrist:
		PlayerType = CounterTrist;
		break;
	}

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	PlayerPawn = GetWorld()->SpawnActor<APlayerBase>(PlayerType, SpawnTrasform, SpawnParam);


	Possess(PlayerPawn);
	PlayerPawn->GetWeaponComponent()->Server_OnSpawn();

	if (IsLocalController())
	{
		PlayerPawn->SetListenHostFlagByController(this);
		bIsListenHost = true;
		OnRep_PlayerPawn();
	}

	bLetClientShowMounse = false;
}

void AWarfareController::ClientRequestStopSequenceToHud_Implementation()
{
	GetHUD<AWarfareHud>()->GetCinematicComponent()->StopAllSequence();
}

void AWarfareController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	if (HasAuthority())
		bLetClientShowMounse = true;
}

void AWarfareController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AWarfareController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AWarfareController::OnRep_PlayerPawn()
{
	OnPlayerSpawned.ExecuteIfBound(PlayerPawn);

	bool IsWorldHasAuthority = HasAuthority();
	bool IsLocal = IsLocalController();

	if (IsWorldHasAuthority)
	{
		if (IsLocal)
			PlayerPawn->SetMultiplayCaseByController(EMultiplayCase::Server_ListenHost);
		else
			PlayerPawn->SetMultiplayCaseByController(EMultiplayCase::Server_Proxy);
	}
	else
	{
		if (IsLocal)
			PlayerPawn->SetMultiplayCaseByController(EMultiplayCase::Client_LocalPlayer);
		else
			PlayerPawn->SetMultiplayCaseByController(EMultiplayCase::Client_Proxy);
	}
}

void AWarfareController::OnRep_bLetClientShowMouns()
{
	bShowMouseCursor = bLetClientShowMounse;
}
