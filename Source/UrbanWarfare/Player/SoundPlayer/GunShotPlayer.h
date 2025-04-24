// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GunShotPlayer.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UGunShotPlayer : public UObject
{
	GENERATED_BODY()
	
public:
	void ExternalInitialize(class APlayerBase* const InOwnerPawn);

private:
	virtual void BeginDestroy() override;
	void InitConstruct();

	UFUNCTION()
	void PlayGunShotSound();
	UFUNCTION()
	void OnWeaponChange(uint8 InWeaponId);

private:
	UPROPERTY()
	TObjectPtr<class UWorld> World;
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	UPROPERTY()
	TObjectPtr<class UCombatComponent> CombatComponent;
	UPROPERTY()
	TObjectPtr<class UWeaponPreLoader> WeaponPreLoader;

	UPROPERTY()
	TObjectPtr<USoundBase> GunShotSound;

	bool bIsInitialized = false;
	FTimerHandle InitTimer;
	int8 InitCount = 10;
};
