// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WarfareController.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API AWarfareController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWarfareController();

	class UUserInterfaceComponent* GetUserInterfaceComponent();

	//class APlayerBase* GeWarfarePawn();
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	UPROPERTY()
	TObjectPtr<class UUserInterfaceComponent> UserInterfaceComponent;

	UPROPERTY()
	TObjectPtr<class UCinematicComponent> CinematicComponent;
};