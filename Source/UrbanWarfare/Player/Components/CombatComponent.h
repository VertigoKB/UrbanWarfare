// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "CombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttack)

UENUM()
enum class EBulletImpactType
{
	Player = 0,
	Other
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URBANWARFARE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	inline void SetLocalFlag(bool bFlag) { bOwnerIsLocal = bFlag; }
	inline void SetbIsReloading(bool bFlag) { bIsReloading = bFlag; }

	inline bool GetbIsNoAmmoInMag() const { return bIsAmmoInMag; }
	// Generally, it is called by AmmoHandler.
	UFUNCTION(Server, Reliable)
	void Server_SetbIsAmmoInMag(bool InIsAmmoInMag);

protected:
	virtual void BeginPlay() override;

private:
	bool InitConstruct();
	void OnSuccessfullyInitialize();

	UFUNCTION()
	void Client_OnStartedInput();
	UFUNCTION()
	void Client_OnCompleteInput();
	UFUNCTION(Server, Reliable)
	void Server_StopContinuousAttack();
	UFUNCTION()
	void OnRep_bAttackFlag();
	UFUNCTION()
	void OnWeaponChange(uint8 InWeaponId);

	void IsServer_ProcessContinuousAttack();

	UFUNCTION(Server, Reliable)
	void Server_ExecuteAttack();
	void ExecuteAttack();
	void Server_PerformAttack();
	void Client_PerformAttack();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayBulletImpact(EBulletImpactType InType, FVector_NetQuantize ImpactLocation, FVector_NetQuantizeNormal ImpactNormal);

public:
	FOnAttack OnAttack;
	bool bOwnerIsListenHost = false;

private:
	UPROPERTY(ReplicatedUsing = OnRep_bAttackFlag)
	bool bAttackFlag = false;
	UPROPERTY(Replicated)
	bool bIsReloading = false;
	UPROPERTY(Replicated)
	bool bIsAmmoInMag = false;

	float RoundInterval = 0.1f;
	float Damage = 0.f;


	EFiringMode FiringMode = EFiringMode::Single;

	FTimerHandle RoundIntervalHandle;
	FTimerHandle StartedInputHandle;

private:
	// Initialization Data
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	//UPROPERTY()
	//TObjectPtr<class APlayerController> OwnerPlayerController;
	UPROPERTY()
	TObjectPtr<class UWarfareAnim> ThirdWarfareAnim;
	UPROPERTY()
	TObjectPtr<class UWarfareAnim> FirstWarfareAnim;
	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;
	UPROPERTY()
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	UPROPERTY()
	TObjectPtr<class UWeaponPreLoader> WeaponPreLoader;
	UPROPERTY()
	TObjectPtr<class UImpactEffectLoader> ImpactEffectLoader;
	UPROPERTY()
	TObjectPtr<class UMuzzleFlashSpawner> MuzzleFlashSpawner;
	UPROPERTY()
	TObjectPtr<class UFireTraceHandler> FireTraceHandler;
	UPROPERTY()
	TObjectPtr<class UAmmoHandler> AmmoHandler;

	bool bAuthority = false;
	bool bIsOwnerPawnControlledByHost = false;
	bool bOwnerIsLocal = false;

	bool bIsInitialized = false;
	FTimerHandle InitHandle;
	uint8 InitCount = 10;

	friend class UFireTraceHandler;

};
