// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ValueBarHud.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UValueBarHud : public UObject
{
	GENERATED_BODY()

public:
	void ExternalInitialize(class UUserWidget* const InOwner, UWorld* const InWorld, class UHealthComponent* const InHealthComp);

private:
	UFUNCTION()
	void OnHealthChange(int8 InValue);
	UFUNCTION()
	void OnArmorChange(int8 InValue);
	
private:
	UPROPERTY()
	TObjectPtr<UWorld> World;
	UPROPERTY()
	TObjectPtr<class UMainWidget> MainWidget;
	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

	float MaxHealth = 1.f;
	float MaxArmor = 1.f;

	int8 MaxUiHealth = 1;
	int8 MaxUiArmor = 1;
};
