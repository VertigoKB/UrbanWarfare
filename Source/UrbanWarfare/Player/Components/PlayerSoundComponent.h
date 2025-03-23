// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSoundComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UPlayerSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;

private:
	UFUNCTION()
	bool InitConstruct();

protected:
	UFUNCTION(Server, Reliable)
	void ServerRequestFootStepsSound();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayFootSteps();

	UFUNCTION()
	void PlayFootStepSound();

	UFUNCTION()
	void StopFootStepSound();

protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> ThePlayer;

	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegInputComp;

	UPROPERTY()
	TObjectPtr<class UPlayerBehaviorComponent> PlayerBehavior;

	UPROPERTY()
	TSoftObjectPtr<USoundBase> FootStepsSound;

	UPROPERTY()
	TObjectPtr<class UBlueprintConfig> BlueprintConfig;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Constants")
	float FootStepSoundInterval = RunInterval;

	float WalkInterval = 1.f;
	float RunInterval = 0.42f;

private:
	FTimerManager* WorldTimer;
	FTimerHandle FootStepHandle;

	bool bPlayingFootSteps = false;

protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//bool DebugFlowA = false;
};