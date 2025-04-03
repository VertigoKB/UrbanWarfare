// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BlueprintConfig.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UBlueprintConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void PostLoad() override;
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|UI")
	TSubclassOf<UUserWidget> MainMenu_DEP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|UI")
	TSubclassOf<UUserWidget> TeamSelection;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|Player")
	TSubclassOf<class APlayerBase> CounterTrist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|Player")
	TSubclassOf<class APlayerBase> Terrorist;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|Weapon")
	TSubclassOf<class AWeaponBase> Pistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|Weapon")
	TSubclassOf<class AWeaponBase> AK47;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|Weapon")
	TSubclassOf<class AWeaponBase> M4A1;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|Audio")
	TObjectPtr<USoundBase> FootStepsSound;



};
