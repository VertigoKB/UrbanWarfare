// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"

UCLASS()
class URBANWARFARE_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	bool PlayerInitilize();

protected:
	// Data Configs
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TObjectPtr<class UMeshConfig> MeshConfig;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	TObjectPtr<class UCharacterMovementComponent> Movement;

};
