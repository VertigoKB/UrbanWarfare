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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SubClass|Animation")
	TSubclassOf<class UAnimInstance> PlayerAnimBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|Audio")
	TObjectPtr<USoundBase> FootStepsSound;
};
