// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UserInterfaceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UUserInterfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUserInterfaceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	bool InitConstruct();

	UFUNCTION()
	void OnClickedGameStart();

protected:

	UPROPERTY()
	TObjectPtr<class AWarfareController> MyOwner;

	UPROPERTY()
	TObjectPtr<class AWarfareHud> MyHud;

	UPROPERTY()
	TObjectPtr<class UMainMenu> MainMenu;

private:
	bool bInitFlag = false;

	FTimerHandle InitTimer;
};
