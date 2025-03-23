// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
//#include "../Common/WarfareLogger.h"

AWarfareGameMode::AWarfareGameMode()
{
    PlayerStartFinder.Reserve(8);
}

void AWarfareGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

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

void AWarfareGameMode::BeginPlay()
{
	Super::BeginPlay();

    IConsoleVariable* MipBiasVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MipMapLODBias"));
    if (MipBiasVar)
    {
        MipBiasVar->Set(1); // 모든 텍스처의 MIP 해상도를 한 단계 낮춤
    }
}
