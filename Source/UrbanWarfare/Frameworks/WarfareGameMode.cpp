// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareGameMode.h"

#include "UrbanWarfare/Frameworks/Components/PlayerSpawnerComponent.h"

AWarfareGameMode::AWarfareGameMode()
{
    //bStartPlayersAsSpectators = true;

    PlayerSpawnerComponent = CreateDefaultSubobject<UPlayerSpawnerComponent>(TEXT("PlayerSpawnerComponent"));
}

UPlayerSpawnerComponent* AWarfareGameMode::GetPlayerSpawnerComponent() { return PlayerSpawnerComponent; }

APlayerController* AWarfareGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, 
    const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    APlayerController* LoginController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

    return LoginController;
}

void AWarfareGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AdjustTextureLevel();
}

void AWarfareGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AWarfareGameMode::AdjustTextureLevel()
{
    IConsoleVariable* MipBiasVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MipMapLODBias"));
    if (MipBiasVar)
        MipBiasVar->Set(1);
}
