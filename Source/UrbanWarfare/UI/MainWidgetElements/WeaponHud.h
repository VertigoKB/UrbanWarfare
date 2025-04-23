// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "UrbanWarfare/Common/CommonEnums.h"
#include "UrbanWarfare/Player/Components/OptionalClasses/SerializedWeaponInventory.h"

#include "WeaponHud.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UWeaponHud : public UObject
{
	GENERATED_BODY()

public:
	void ExternalInitialize(class UImage* InImageRifle, class UImage* InImagePistol, UWorld* InWorld);

	UFUNCTION()
	void OnInventoryChange(const TArray<FInventoryItem>& InInventory);

	UFUNCTION()
	void OnEquipWeapon(EWeaponType InType);

protected:

	virtual void BeginDestroy() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImageRifle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImagePistol;

	UPROPERTY()
	TObjectPtr<UWorld> World;

	UPROPERTY()
	TObjectPtr<class UWeaponPreLoader> WeaponPreLoader;
};
