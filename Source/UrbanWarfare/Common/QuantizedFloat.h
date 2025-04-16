// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuantizedFloat.generated.h"

USTRUCT()
struct FQuantizedFloat
{
	GENERATED_BODY()

	uint16 CompressedValue;

	float GetFloat() const { return CompressedValue / 100.f; }
	void SetFloat(float InValue) { CompressedValue = FMath::RoundToInt(InValue * 100.f); }

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << CompressedValue;
		bOutSuccess = true;
		return true;
	}
};
