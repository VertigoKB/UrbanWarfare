// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

DECLARE_DELEGATE(FOnFire)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void Server_SetbIsReloading(bool bFlag);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool InitConsruct();
	void OnInitializationComplete();
	void OnTriggerAttack();
	void OnCompleteAttack();

	UFUNCTION(Server, Reliable)
	void Server_TriggerAttack();

	void AttackLineTrace();

	void OnWeaponChange();

	UFUNCTION()
	void OnRep_bAutoModeEffectFlag();
	UFUNCTION()
	void OnRep_bSingleModeEffectFlag();

public:
	FOnFire OnFire;

private:
	UPROPERTY(ReplicatedUsing = OnRep_bAutoModeEffectFlag)
	bool bAutoModeEffectFlag = false;

	// This replicated variable is used solely to invoke a callback; its boolean value carries no intrinsic meaning.
	UPROPERTY(ReplicatedUsing = OnRep_bSingleModeEffectFlag)
	bool bSingleModeEffectFlag = false;	

	UPROPERTY(Replicated)
	bool bIsReloading = false;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> ComponentToPlay;
	UPROPERTY()
	TObjectPtr<UParticleSystem> ParticleToPlay;

	
	bool bIsAttackTriggered = false;
	bool bSingleAttackActive = true;	// SingleAttack cool time.
	float Damage = 0.f;
	float AttackInterval = 0.1f;
	FTimerHandle TriggerHandle;
	FTimerHandle RoundIntervalHandle;
	FTimerHandle EffectHandle;
	

private:
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

	bool bInitFlag = false;
	uint8 InitCount = 10;
	FTimerHandle InitTimer;

//public:
//	UPROPERTY(BlueprintReadWrite)
//	bool DebugFlowA = false;
//	UPROPERTY(BlueprintReadWrite)
//	bool DebugFlowB = false;
//	UPROPERTY(BlueprintReadWrite)
//	bool DebugFlowC = false;
//	UPROPERTY(BlueprintReadWrite)
//	bool DebugFailedInitA = false;
//	UPROPERTY(BlueprintReadWrite)
//	bool DebugFailedInitB = false;
//	UPROPERTY(BlueprintReadWrite)
//	bool DebugFailedInitC = false;
//	UPROPERTY(BlueprintReadWrite)
//	bool DebugFailedInitD = false;
//	UPROPERTY(BlueprintReadWrite)
//	bool DebugFailedInitE = false;

	

		
};
