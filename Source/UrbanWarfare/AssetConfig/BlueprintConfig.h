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
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|UI")
	//TSubclassOf<UUserWidget> MainMenu_DEP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|UI")
	TSubclassOf<UUserWidget> TeamSelection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class|UI")
	TSubclassOf<UUserWidget> MainWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|Audio")
	TObjectPtr<USoundBase> FootStepsSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> TerroristReloadPistol;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> TerroristReloadRifle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> TerroristFirePistol;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> TerroristFireRifle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> TerroristDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> CounterTristReloadPistol;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> CounterTristReloadRifle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> CounterTristFirePistol;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> CounterTristFireRifle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints|AnimMontage")
	TObjectPtr<UAnimMontage> CounterTristDeath;
};
