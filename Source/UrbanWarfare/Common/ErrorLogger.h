// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

static const FColor LogColor = FColor::FromHex(TEXT("8241CAFF"));

#define LOG_NULL(Var) \
	if (!(Var)) \
	{ \
		FString LogMessage = FString::Printf(TEXT("[%s_%d]Null pointer detected: %s"), TEXT(__FUNCTION__), __LINE__, TEXT(#Var)); \
		if (GEngine)\
			GEngine->AddOnScreenDebugMessage(-1, 60.f, LogColor, LogMessage); \
	}