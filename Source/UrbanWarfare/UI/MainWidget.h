// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UENUM()
enum class EMainWidgetElem : uint8
{
	ImageRifle = 0,
	ImagePistol,
	AmmoCondition,
	RemainAmmo,
	ExtraAmmo
};

UCLASS()
class URBANWARFARE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidgetVisibility(EMainWidgetElem TargetWidget, ESlateVisibility InVisibility);
	void SetTextBlockContent(EMainWidgetElem TargetWidget, FText InContent);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
private:
	//bool InitConstruct();

	UFUNCTION()
	void OnPlayerSpawned(class APlayerBase* InPlayer);
private:
	// WeaponHud
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImageRifle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImagePistol;

private:
	// AmmoHud
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> AmmoCondition;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RemainAmmo;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ExtraAmmo;

private:
	UPROPERTY()
	TObjectPtr<class UWeaponHud> WeaponHud;
	UPROPERTY()
	TObjectPtr<class UAmmoHud> AmmoHud;

	UPROPERTY()
	TMap<EMainWidgetElem, UWidget*> WidgetsMap;

private:	// Cach Data
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	
	FTimerHandle InitHandle;
	uint8 InitCount = 10;
};
