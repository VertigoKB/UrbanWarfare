// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPtionalClasses/SerializedMovementState.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "PlayerBehaviorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UPlayerBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBehaviorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	bool InitConstruct();

	void OnSuccessInitialize();
protected:
	UFUNCTION()
	void OnRep_MovementState();

	UFUNCTION(Server, Reliable)
	void ServerApplySpeed();

protected:
	// Crouch

	UFUNCTION()
	void TriggerCrouch(bool bCrouch);

	UFUNCTION(Server, Reliable)
	void ServerCrouch(bool bCrouch);

protected:
	// Walk

	UFUNCTION()
	void TriggerWalk(bool bWalk);

	UFUNCTION(Server, Unreliable)
	void ServerWalk(bool bWalk);

protected:
	// Jump

	UFUNCTION()
	void TriggerJump(bool Jump);

protected:
	// Look
	UFUNCTION()
	void TriggerLook();

	UFUNCTION(Server, Unreliable)
	void ServerLook();

	UFUNCTION()
	void VerifyNecessitySyncAimDirection();
	
	UFUNCTION()
	uint8 CalcAimDirection();

	inline float DecompressePitch(uint8 CompressedPitch) {
		return (CompressedPitch / 255.f) * 180.f - 90.f;
	}
	
protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> ThePlayer;

	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegInputComp;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> PlayerMovement;

	UPROPERTY()
	TObjectPtr<class UCombatComponent> CombatComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Constant")
	float WalkSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Constant")
	float RunSpeed = 750.f;

	UPROPERTY(Replicated)
	float CurrentSpeed = RunSpeed;

public:
	UPROPERTY(ReplicatedUsing = OnRep_MovementState)
	FMovementStateArray MovementState;

	UPROPERTY(Replicated)
	float AimDirection = 0.f;

public:

	bool bInitFlag = false;

private:
	FTimerHandle SyncAimTimer;

	FTimerHandle FallingChecker;

	FTimerHandle InitTimer;

	uint8 InitCount = 1;

public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//int32 DebugA;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//float DebugF;
};
