// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	bool InitConstruct();
	void OnSuccessfullyInitialize();

	UFUNCTION()
	void OnStartedAttack();
	UFUNCTION()
	void OnCompleteAttack();
	UFUNCTION()
	void OnWeaponChange();

	bool AttackLineTrace();
	void SpawnMuzzleFlash();

private:
	UFUNCTION(Server, Reliable)
	void Server_RequestAttack();

	void Client_PerformAttack();

	

private:
	UPROPERTY(Replicated)
	bool bIsAttacking = false;

	FTimerHandle StartedInputHandle;

private:
	// Initialization Data
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class APlayerController> OwnerPlayerController;
	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	UPROPERTY()
	TObjectPtr<class UWeaponPreLoader> WeaponPreLoader;
	UPROPERTY()
	TObjectPtr<class UMuzzleFlashSpawner> MuzzleFlashSpawner;
	UPROPERTY()
	TObjectPtr<class UFireTraceHandler> FireTraceHandler;

	bool bAuthority = false;
	bool bIsListen = false;

	bool bIsInitialized = false;
	FTimerHandle InitHandle;
	uint8 InitCount = 10;
		
};
