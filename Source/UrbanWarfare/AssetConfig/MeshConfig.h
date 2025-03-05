// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshConfig.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UMeshConfig : public UDataAsset
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterMesh")
	TObjectPtr<class USkeletalMesh> Terrorist;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterMesh")
	TObjectPtr<class USkeletalMesh> CounterTerrorist;
};
