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

	void RequestStopSequenceToHud();
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UUserInterfaceComponent> UserInterfaceComponent;
};