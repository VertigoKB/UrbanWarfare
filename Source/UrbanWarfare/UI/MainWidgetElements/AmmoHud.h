// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AmmoHud.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UAmmoHud : public UObject
{
	GENERATED_BODY()
	
public:
	void ExternalInitialize(class UUserWidget* const InOwner, UWorld* const InWorld);

protected:
	virtual void BeginDestroy() override;


private:
	UPROPERTY()
	TObjectPtr<class UMainWidget> MainWidget;
	UPROPERTY()
	TObjectPtr<UWorld> World;
	UPROPERTY()
	TObjectPtr<class UAmmoHandler> AmmoHandler;

	FTimerHandle InitHandle;
	
};
