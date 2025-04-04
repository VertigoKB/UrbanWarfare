// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "WeaponDataTableRow.generated.h"

USTRUCT()
struct FWeaponDataTableRow
{
	GENERATED_BODY()

	UPROPERTY()
	FString WeaponName;

	UPROPERTY()
	EWeaponType WeaponType;

	UPROPERTY()
	TObjectPtr<USkeletalMesh> WeaponMesh;

};