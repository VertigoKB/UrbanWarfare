// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintConfig.h"
#include "../Common/WarfareLogger.h"

void UBlueprintConfig::PostLoad()
{
	///Script/Engine.AnimBlueprint''

	Super::PostLoad();

	LOG_NULL(PlayerAnimBP);
}
