// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool InitConsruct();
	void OnInitializationComplete();

	UFUNCTION(Server, Reliable)
	void Server_TriggerAttack();

	void AttackLineTrace();

	void OnWeaponChange();
public:	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

protected:

private:
	bool bIsTriggeredAttack = false;
	float Damage = 0.f;
	float AttackInterval = 0.1f;
	FTimerHandle RoundIntervalHandle;
	

private:
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class AWarfareController> OwnerPlayerController;
	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	UPROPERTY()
	TObjectPtr<class UWeaponPreLoader> WeaponPreLoader;

	bool bInitFlag = false;
	uint8 InitCount = 10;
	FTimerHandle InitTimer;
		
};
