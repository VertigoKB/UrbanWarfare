// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "../../AssetConfig/InputConfig.h"
#include "RegisterInputComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnInputCrouch, bool)
DECLARE_DELEGATE_OneParam(FOnInputWalk, bool)
DECLARE_DELEGATE_OneParam(FOnInputJump, bool)
DECLARE_DELEGATE(FOnInputLook)
DECLARE_DELEGATE(FOnFootStepPlay)
DECLARE_DELEGATE(FOnStopPlayingFootStep)
DECLARE_DELEGATE(FOnTestInput)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API URegisterInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URegisterInputComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UFUNCTION()
	bool CachAndInit();
	UFUNCTION()
	void SetupEnhancedInput();
	
protected:
	UFUNCTION()
	void InputMove(const FInputActionValue& Value);

	UFUNCTION()
	void TempMove(FVector2D Input);

	UFUNCTION()
	void CompleteMove(const FInputActionValue& Value);

	UFUNCTION()
	void InputLook(const FInputActionValue& Value);

	UFUNCTION()
	void InputCrouch(const FInputActionValue& Value);

	UFUNCTION()
	void InputWalk(const FInputActionValue& Value);

	UFUNCTION()
	void InputJump(const FInputActionValue& Value);

	UFUNCTION()
	void InputTest(const FInputActionValue& Value);

private:
	UFUNCTION()
	void TempTest();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputConfig")
	TObjectPtr<class UInputConfig> InputConfig;

	bool bInputConfigChecker = false;
protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> PlayerPawn;
	UPROPERTY()
	TObjectPtr<APlayerController> MyController;

public:
	FOnInputCrouch OnInputCrouch;
	FOnInputWalk OnInputWalk;
	FOnInputJump OnInputJump;
	FOnInputLook OnInputLook;

public:
	FOnFootStepPlay OnFootStepPlay;
	FOnStopPlayingFootStep OnStopPlayingFootStep;

public:
	FOnTestInput OnTestInput;


	bool bInitFlag = false;

private:
	bool TestAutoMove = false;

	uint8 InitCount = 1;

	FTimerHandle InitTimer;
};
