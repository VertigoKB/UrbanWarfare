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
	ExtraAmmo,
	HealthBar,
	HealthValue,
	ArmorBar,
	ArmorValue
};

class UImage;
class UOverlay;
class UTextBlock;
class UProgressBar;

UCLASS()
class URBANWARFARE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidgetVisibility(EMainWidgetElem TargetWidget, const ESlateVisibility InVisibility);
	void SetTextBlockContent(EMainWidgetElem TargetWidget, const FText InContent);
	void SetProgressBarPercentValue(EMainWidgetElem TargetWidget, const float InValue);

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
	TObjectPtr<UImage> ImageRifle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImagePistol;

private:
	// AmmoHud
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> AmmoCondition;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RemainAmmo;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExtraAmmo;

private:
	// Health And Armor
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthValue;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ArmorValue;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ArmorBar;

private:
	UPROPERTY()
	TObjectPtr<class UWeaponHud> WeaponHud;
	UPROPERTY()
	TObjectPtr<class UAmmoHud> AmmoHud;
	UPROPERTY()
	TObjectPtr<class UValueBarHud> ValueBarHud;

	UPROPERTY()
	TMap<EMainWidgetElem, UWidget*> WidgetsMap;

private:	// Cach Data
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	
	FTimerHandle InitHandle;
	uint8 InitCount = 10;
};
