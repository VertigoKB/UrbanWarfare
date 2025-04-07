// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#include "../AssetConfig/MeshConfig.h"
#include "../AssetConfig/BlueprintConfig.h"

#include "PlayerBase.generated.h"

UCLASS()
class URBANWARFARE_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBase();

	class URegisterInputComponent* GetRegInputComp() const;
	class UPlayerBehaviorComponent* GetPlayerBehavior() const;
	class UPlayerSoundComponent* GetSoundPlayer() const;
	class USkeletalMeshComponent* GetTheMesh() const;
	class USkeletalMeshComponent* GetRifleMesh() const;
	class USkeletalMeshComponent* GetPistolMesh() const;
	class UWeaponComponent* GetWeaponComponent() const;
	class UBlueprintConfig* GetBlueprintConfig() const;
	bool IsPlayerFalling() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void SetupBasicComponents();

	UFUNCTION()
	void SetupCustomComponents();

	UFUNCTION()
	void SetupMesh();
	
public:



public:
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Mesh")
	TObjectPtr<class USkeletalMeshComponent> TheThirdMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mesh")
	TObjectPtr<class USkeletalMeshComponent> TheRifleMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mesh")
	TObjectPtr<class USkeletalMeshComponent> ThePistolMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mesh")
	TObjectPtr<class USkeletalMeshComponent> RifleHandMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mesh")
	TObjectPtr<class USkeletalMeshComponent> PistolHandMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class USpringArmComponent> TheSpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> TheCamera;

protected:
	// Custom Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Components")
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Components")
	TObjectPtr<class UPlayerBehaviorComponent> PlayerBehavior;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Components")
	TObjectPtr<class UPlayerSoundComponent> PlayerSoundComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Components")
	TObjectPtr<class UWeaponComponent> WeaponComponent;
protected:
	// Editable Constant

	UPROPERTY(EditDefaultsOnly, Category = "UrbanWarfare")
	bool DebugCamera = false;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void TestUrban();
};
