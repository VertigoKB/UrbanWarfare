// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

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
	virtual UObject* GetPlayerBehavior() = 0;
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

protected:
	UFUNCTION()
	void SetupBasicComponents();

	UFUNCTION()
	void SetupCustomComponents();

public:
	UFUNCTION(Server, Reliable)
	void ServerCrouch(bool bCrouch);

public:
	inline bool GetHasAuthority() { return HasAuthority(); }
	
public:
	virtual UActorComponent* GetRegInputComp() override;
	virtual UObject* GetPlayerBehavior() ;

protected:
	// Data Configs

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TObjectPtr<class UMeshConfig> MeshConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TObjectPtr<class UBlueprintConfig> BlueprintConfig;

protected:
	// Basic Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Capsule")
	TObjectPtr<class UCapsuleComponent> TheCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	TObjectPtr<class UCharacterMovementComponent> TheMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Mesh")
	TObjectPtr<class USkeletalMeshComponent> TheMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class USpringArmComponent> TheSpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> TheCamera;

protected:
	// Custom Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Components")
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;

protected:
	// Custom Objects

	UPROPERTY()
	TObjectPtr<class UPlayerBehavior> PlayerBehavior;
};
