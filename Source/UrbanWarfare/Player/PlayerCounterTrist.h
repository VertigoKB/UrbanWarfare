// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "PlayerCounterTrist.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API APlayerCounterTrist : public APlayerBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
