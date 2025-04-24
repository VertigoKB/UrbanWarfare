// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FireTraceHandler.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UFireTraceHandler : public UObject
{
	GENERATED_BODY()
	
public:
	void ExternalInitialize(class APlayerBase* InRootOwner);
	virtual void BeginDestroy() override;

	bool AttackLineTrace();
private:
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class UWorld> World;


	FTimerHandle AutoFireHandle;

	bool bAuthority = false;
};
