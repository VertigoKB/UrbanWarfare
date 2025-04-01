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


	UFUNCTION()
	void OnReleaseGameStart();

private:
	bool InitConstruct();

	UFUNCTION(Server, Reliable)
	void ServerRequestSpawnPlayer();

protected:

	UPROPERTY()
	TObjectPtr<class AWarfareController> MyOwner;

	UPROPERTY()
	TObjectPtr<class AWarfarePlayerState> OwnerPlayerState;

	UPROPERTY()
	TObjectPtr<class AWarfareHud> MyHud;

	UPROPERTY()
	TObjectPtr<class UMainMenu> MainMenu;

	FTimerHandle InitTimer;
	uint8 InitCount = 10;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugBoolA = false;

};
