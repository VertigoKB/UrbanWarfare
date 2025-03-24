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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	bool InitConstruct();

protected:

	UFUNCTION()
	void PlayFootStepSound(const float SpeedSquared);
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
	TSoftObjectPtr<USoundBase> FootStepRef;

	UPROPERTY()
	TObjectPtr<class UBlueprintConfig> BlueprintConfig;

	bool bAuthority = false;

protected:
	float FootStepSoundInterval = RunInterval;
	float WalkInterval = 1.f;
	float RunInterval = 0.42f;

	float MinVelocitySquared = 40.f * 40.f;

	bool bPlayingFootSteps = false;
private:
	UPROPERTY()
	USoundBase* LoadedFootSteps;

	FTimerManager* WorldTimer;
	FTimerHandle FootStepHandle;

};