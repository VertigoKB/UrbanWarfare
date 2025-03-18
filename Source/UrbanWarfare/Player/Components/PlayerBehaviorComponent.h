// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Running = 0,
	Walking,
	Crouching
};


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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UFUNCTION()
	bool InitConstruct();

protected:
	UFUNCTION(Server, Reliable)
	void ServerExecuteApplyingMovementState();

protected:
	// Crouch

	UFUNCTION()
	void TriggerCrouch(bool bCrouch);

	UFUNCTION()
	void ExecuteCrouch(bool bCrouch);

	UFUNCTION(Server, Reliable)
	void ServerCrouch(bool bCrouch);

protected:
	// Walk

	UFUNCTION()
	void ExecuteWalk(bool bWalk);

	UFUNCTION(Server, Unreliable)
	void ServerWalk(bool bWalk);

protected:
	// Jump

	UFUNCTION()
	void ExecuteJump(bool Jump);
	
protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> ThePlayer;

	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegInputComp;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> PlayerMovement;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Constant")
	float WalkSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Constant")
	float RunSpeed = 600.f;

public:
	UPROPERTY(Replicated)
	TArray<EMovementState> MovementState;

};
