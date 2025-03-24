// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
//#include "../Common/WarfareLogger.h"

AWarfareGameMode::AWarfareGameMode()
{
    //bStartPlayersAsSpectators = true;
    PlayerStartFinder.Reserve(8);

}

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
    RestorePlacedPlayerStart();
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

void AWarfareGameMode::RestorePlacedPlayerStart()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartFinder);

    for (uint8 i = 0; i < PlayerStartFinder.Num(); i++)
    {
        if (PlayerStartFinder[i]->Tags.Contains("CounterTrist"))
            CounterTristStart.Add(PlayerStartFinder[i]);
        else if (PlayerStartFinder[i]->Tags.Contains("Terrorist"))
            TerroristStart.Add(PlayerStartFinder[i]);
    }

    PlayerStartFinder.Empty();
}
