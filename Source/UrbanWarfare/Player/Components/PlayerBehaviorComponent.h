// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UFUNCTION()
	bool InitConstruct();

protected:
	// Crouch

	UFUNCTION()
	void ExecuteCrouch(bool bCrouch);

	UFUNCTION(Server, Reliable)
	void ServerCrouch(bool bCrouch);

	UFUNCTION(Client, Reliable)
	void ClientCrouch(bool bCrouch);

protected:
	// Walk

	UFUNCTION()
	void ExecuteWalk(bool bWalk);
	UFUNCTION(Server, Reliable)
	void ServerWalk(bool bWalk);
	UFUNCTION(Client, Reliable)
	void ClientWalk(bool bWalk);
	

public:
	UPROPERTY(Replicated)
	bool bCrouching = false;

	UPROPERTY(Replicated)
	bool bWalking = false;

protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> ThePlayer;

	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegInputComp;

protected:
	//UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Constant")
	//float WalkSpeed = 200.f;
	//UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Constant")
	//float RunSpeed = 600.f;

};
