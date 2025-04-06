// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WeaponPreLoader.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UWeaponPreLoader : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	bool LoadWeaponDataAsset();

	class UWeaponDataAsset* GetWeaponDataByName(FName InName);

	UPROPERTY()
	TMap<FName, UWeaponDataAsset*> WeaponDataMap;

	bool bIsLoadSuccess = false;
};
