// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "CommonStructs.generated.h"

// This class does not need to be modified.
USTRUCT()
struct FDroppedWeaponData
{
	GENERATED_BODY()

public:
	uint8 WeaponId = 0;
	EWeaponType WeaponType = EWeaponType::UnArmed;
	uint16 AmmoInMag = 0;
	uint16 ExtraAmmo = 0;
};

USTRUCT()
struct FWeaponAmmoData
{
	GENERATED_BODY()

public:
	uint16 AmmoInMag = 0;
	uint16 ExtraAmmo = 0;
};