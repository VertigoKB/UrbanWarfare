// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

static const FColor LogColor = FColor::FromHex(TEXT("8241CAFF"));

#define LOG_NULL_BEGIN(Var) \
	if (!(Var)) \
	{ \
		FString LogMessage = FString::Printf(TEXT("UWLogger: [%s_%d]Null pointer detected: %s"), TEXT(__FUNCTION__), __LINE__, TEXT(#Var)); \
		if (GEngine)\
			GEngine->AddOnScreenDebugMessage(-1, 60.f, LogColor, LogMessage); \
	}

#define LOG_NULL(Var) \
	if (!(Var)) \
	{ \
		UE_LOG(LogTemp, Log, TEXT("UWLogger: [%s_%d]Null pointer detected: %s"), TEXT(__FUNCTION__), __LINE__, TEXT(#Var)); \
	}

#define LOG_SIMPLE(Input) \
	UE_LOG(LogTemp, Log, TEXT("UWLogger: %s"), *FString(Input));

#define LOG_SPFUNC(Input) \
	UE_LOG(LogTemp, Log, TEXT("UWLogger: [%s_%d] %s"), TEXT(__FUNCTION__), __LINE__, *FString(Input));