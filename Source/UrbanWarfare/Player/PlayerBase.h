// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#include "../AssetConfig/MeshConfig.h"
#include "../AssetConfig/BlueprintConfig.h"

#include "PlayerBase.generated.h"

UINTERFACE(MinimalAPI)
class UDataProvider : public UInterface
{
	GENERATED_BODY()
};
class URBANWARFARE_API IDataProvider
{
	GENERATED_BODY()

public:
	virtual UActorComponent* GetRegInputComp() = 0;
	virtual UActorComponent* GetPlayerBehavior() = 0;
};


UCLASS()
class URBANWARFARE_API APlayerBase : public ACharacter, public IDataProvider
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void SetupBasicComponents();

	UFUNCTION()
	void SetupCustomComponents();

public:
	inline bool GetHasAuthority() { return HasAuthority(); }
	
public:
	UFUNCTION(Server, Reliable)
	void ServerSetWalkSpeed(bool bWalk);
public:
	virtual UActorComponent* GetRegInputComp() override;
	virtual UActorComponent* GetPlayerBehavior() ;

protected:
	UFUNCTION()
	void OnRep_ChangeMaxWalkSpeed();

protected:
	// Data Configs

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TObjectPtr<class UMeshConfig> MeshConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TObjectPtr<class UBlueprintConfig> BlueprintConfig;

public:
	// Public Basic Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	TObjectPtr<class UCharacterMovementComponent> TheMovement;

protected:
	//Protected Basic Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Capsule")
	TObjectPtr<class UCapsuleComponent> TheCapsule;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Mesh")
	TObjectPtr<class USkeletalMeshComponent> TheMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class USpringArmComponent> TheSpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> TheCamera;

protected:
	// Custom Components

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Components")
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Components")
	TObjectPtr<class UPlayerBehaviorComponent> PlayerBehavior;

protected:
	// Constants
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float WalkSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RunSpeed = 600.f;

	UPROPERTY(ReplicatedUsing = OnRep_ChangeMaxWalkSpeed)
	float CurrentSpeed = RunSpeed;
};
