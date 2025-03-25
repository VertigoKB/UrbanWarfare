// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BlueprintConfig.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UBlueprintConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void PostLoad() override;
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SubClass|UI")
	TSubclassOf<UUserWidget> MainMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SubClass|Player")
	TSubclassOf<class APlayerBase> CounterTrist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SubClass|Player")
	TSubclassOf<class APlayerBase> Terrorist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|Audio")
	TObjectPtr<USoundBase> FootStepsSound;
};
