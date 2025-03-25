// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfarePlayerState.h"
#include "WarfareGameMode.h"
#include "Net/UnrealNetwork.h"
#include "UrbanWarfare/Frameworks/WarfareGameMode.h"

void AWarfarePlayerState::BeginPlay()
{
	Super::BeginPlay();

}

void AWarfarePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AWarfarePlayerState, bClickedGameStart);
}

void AWarfarePlayerState::OnClickedGameStart_Implementation()
{
	AWarfareGameMode* AuthGameMode = GetWorld()->GetAuthGameMode<AWarfareGameMode>();
	
	if (AuthGameMode)
		AuthGameMode->SpawnPlayerByPlayerState(this);
}
