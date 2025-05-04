// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETeam : uint8
{
	None = 0,
	Terrorist,
	CounterTrist
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	UnArmed = 0			UMETA(DisplayName = "UnArmed"),
	Pistol				UMETA(DisplayName = "Pistol"),
	Rifle				UMETA(DisplayName = "Rifle")
};

UENUM()
enum class EWeaponItem : uint8
{
	None = 0,
	Pistol,
	AK47,
	M4A1
};

UENUM()
enum class EFiringMode : uint8
{
	Single = 0,
	Auto,
	Burst,
	Manual
};

UENUM()
enum class EMultiplayCase : uint8
{
	Unspecified,
	Server_ListenHost,
	Server_Proxy,
	Client_LocalPlayer,
	Client_Proxy
};