// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfarePlayerState.h"
#include "WarfareGameMode.h"
#include "Net/UnrealNetwork.h"
#include "UrbanWarfare/Frameworks/WarfareGameMode.h"

AWarfarePlayerState::AWarfarePlayerState()
{
}

void AWarfarePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AWarfarePlayerState::BeginPlay()
{
	Super::BeginPlay();

}