// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"


UCLASS()
class URBANWARFARE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
private:
	bool InitConstruct();

	UFUNCTION()
	void OnPlayerSpawned(class APlayerBase* InPlayer);
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImageRifle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImagePistol;

private:
	UPROPERTY()
	TObjectPtr<class UWeaponHud> WeaponHud;

private:	// Cach Data
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	
	FTimerHandle InitHandle;
	uint8 InitCount = 10;
};
