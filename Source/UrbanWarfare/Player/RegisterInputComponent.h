// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "../AssetConfig/InputConfig.h"
#include "RegisterInputComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnInputCrouch, bool)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API URegisterInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URegisterInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	bool CachAndInit();
	UFUNCTION()
	void SetupEnhancedInput();
	
protected:
	UFUNCTION()
	void InputMove(const FInputActionValue& Value);

	UFUNCTION()
	void InputLook(const FInputActionValue& Value);

	UFUNCTION()
	void InputCrouch(const FInputActionValue& Value);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputConfig")
	TObjectPtr<class UInputConfig> InputConfig;

	bool bInputConfigChecker = false;
protected:
	UPROPERTY()
	TObjectPtr<APawn> PlayerPawn;
	UPROPERTY()
	TObjectPtr<APlayerController> MyController;

public:
	FOnInputCrouch OnInputCrouch;
};
