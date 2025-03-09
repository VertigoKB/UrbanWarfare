// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareGameMode.h"

AWarfareGameMode::AWarfareGameMode()
{
}

void AWarfareGameMode::BeginPlay()
{
	Super::BeginPlay();

    //IConsoleVariable* MipBiasVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MipMapLODBias"));
    //if (MipBiasVar)
    //{
    //    MipBiasVar->Set(2); // 모든 텍스처의 MIP 해상도를 한 단계 낮춤
    //}
}
