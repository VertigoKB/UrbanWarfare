// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETeam : uint8
{
	Terrorist = 0,
	CounterTrist
};

UENUM(BlueprintType)
enum class EWeaponeType : uint8
{
	UnArmed = 0			UMETA(DisplayName = "UnArmed"),
	Pistol				UMETA(DisplayName = "Pistol"),
	Rifle				UMETA(DisplayName = "Rifle")
};