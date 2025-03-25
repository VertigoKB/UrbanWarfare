// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Frameworks/WarfarePlayerState.h"
#include "UrbanWarfare/Player/PlayerBase.h"
//#include "../Common/WarfareLogger.h"

AWarfareGameMode::AWarfareGameMode()
{
    //bStartPlayersAsSpectators = true;
    PlayerStartFinder.Reserve(8);
    CounterTristStart.Reserve(4);
    TerroristStart.Reserve(4);
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

void AWarfareGameMode::SpawnPlayerByPlayerState(AWarfarePlayerState* PlayerState)
{
    APlayerController* OwnerController = Cast<APlayerController>(PlayerState->GetOwner());
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    if (OwnerController)
    {
        uint8 RandomTeam = FMath::RandRange(0, 1);
        uint8 RandomIndex = FMath::RandRange(0, 3);

        switch (RandomTeam)
        {
        case 0:
        {
            FVector SpawnLocation = CounterTristStart[RandomIndex]->GetActorLocation();
            FRotator SpawnRotation = CounterTristStart[RandomIndex]->GetActorRotation();

            APawn* Target = GetWorld()->SpawnActor<APawn>(BlueprintConfig->CounterTrist, SpawnLocation, SpawnRotation, SpawnParams);

            OwnerController->Possess(Target);
            break;
        }
        case 1:
        {
            FVector SpawnLocation = TerroristStart[RandomIndex]->GetActorLocation();
            FRotator SpawnRotation = TerroristStart[RandomIndex]->GetActorRotation();

            APawn* Target = GetWorld()->SpawnActor<APawn>(BlueprintConfig->CounterTrist, SpawnLocation, SpawnRotation, SpawnParams);

            OwnerController->Possess(Target);
            break;
        }
        }
    }

    //PlayerState->PlayerGameCondition = EPlayerGameCondition::InGame;
}
