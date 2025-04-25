// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MuzzleFlashSpawner.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UMuzzleFlashSpawner : public UObject
{
	GENERATED_BODY()
	
public:
	void ExternalInitialize(class APlayerBase* const InRootOwner);

	void SetTargetComponent(uint8 InWeaponId);
	void PlayMuzzleEffect();
private:
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TWeakObjectPtr<UWorld> World;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> ComponentToPlay;
	UPROPERTY()
	TObjectPtr<UParticleSystem> ParticleToPlay;
};
