// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

static const FColor LogColor = FColor::FromHex(TEXT("8241CAFF"));

DECLARE_LOG_CATEGORY_EXTERN(LogWarfare, Log, All);

#define LOG_NULL_BEGIN(Var) \
	if (!(Var)) \
	{ \
		FString LogMessage = FString::Printf(TEXT("[%s_%d] Null pointer detected: %s"), TEXT(__FUNCTION__), __LINE__, TEXT(#Var)); \
		if (GEngine)\
			GEngine->AddOnScreenDebugMessage(-1, 15.f, LogColor, LogMessage); \
		UE_LOG(LogWarfare, Log, TEXT("[%s_%d]Null pointer detected: %s"), TEXT(__FUNCTION__), __LINE__, TEXT(#Var)); \
	}

#define LOG_NULL(Var) \
	if (!(Var)) \
	{ \
		UE_LOG(LogWarfare, Log, TEXT("[%s_%d]Null pointer detected: %s"), TEXT(__FUNCTION__), __LINE__, TEXT(#Var)); \
	}

#define LOG_SIMPLE(Format, ...) \
	UE_LOG(LogWarfare, Log, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_WARNSIMP(Format, ...) \
	UE_LOG(LogWarfare, Warning, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_EFUNC(Format, ...) \
	UE_LOG(LogWarfare, Log, TEXT("[%s_%d] %s"), TEXT(__FUNCTION__), __LINE__, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_WARNFUNC(Format, ...) \
	UE_LOG(LogWarfare, Warning, TEXT("[%s_%d] %s"), TEXT(__FUNCTION__), __LINE__, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_NETINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT_%d"), GPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))

#define LOG_NETSIMPLE(Format, ...) \
	UE_LOG(LogWarfare, Log, TEXT("[%s] %s"), LOG_NETINFO,*FString::Printf(Format, ##__VA_ARGS__))

#define LOG_NETEFUNC(Format, ...) \
	UE_LOG(LogWarfare, Log, TEXT("[%s] [%s_%d] %s"), TEXT(__FUNCTION__), LOG_NETINFO, __LINE__, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_NETINFO_UOBJECT(World) ((World->GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT_%d"), GPlayInEditorID) : ((World->GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))

#define LOG_NETSIMPLE_UOBJECT(World, Format, ...) \
	UE_LOG(LogWarfare, Log, TEXT("[%s] %s"), LOG_NETINFO_UOBJECT(World),*FString::Printf(Format, ##__VA_ARGS__))

#define LOG_NETEFUNC_UOBJECT(World, Format, ...) \
	UE_LOG(LogWarfare, Log, TEXT("[%s] [%s_%d] %s"), LOG_NETINFO_UOBJECT(World), TEXT(__FUNCTION__), __LINE__, *FString::Printf(Format, ##__VA_ARGS__))