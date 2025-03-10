// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerBase.h"
#include "PlayerBehavior.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UPlayerBehavior : public UObject
{
	GENERATED_BODY()
	
public:
	UPlayerBehavior();
	
public:

	UFUNCTION()
	void ExecuteCrouch(bool bCrouch);

public:
	bool bCrouching = false;

private:
	bool bInitFlag = false;

	UPROPERTY()
	TObjectPtr<UWorld> TheWorld;

	UPROPERTY()
	TObjectPtr<APlayerBase> ThePlayer;

	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegInputComp;

};
