// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamSelection.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UTeamSelection : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickedRedTeam();

	UFUNCTION()
	void OnClickedBlueTeam();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ButtonTerrorist;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ButtonCounterTrist;
};
