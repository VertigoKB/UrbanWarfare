// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WarfareHud.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API AWarfareHud : public AHUD
{
	GENERATED_BODY()

public:
	AWarfareHud();

	class UCinematicComponent* GetCinematicComponent();
	class UTeamSelection* GetTeamSelectionWidget();
	//class UMainMenu* GetWidgetMainMenu();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void CreateTeamSelectionWidget();
	void CreateMainWidget();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBlueprintConfig> BlueprintConfig;

	UPROPERTY()
	TObjectPtr<class UTeamSelection> TeamSelection;

	UPROPERTY()
	TObjectPtr<class UMainWidget> MainWidget;

	UPROPERTY()
	TObjectPtr<class UCinematicComponent> CinematicComponent;

};
