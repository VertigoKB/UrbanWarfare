// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WarfareAnim.generated.h"

/**
 * 
 */
UCLASS()
class URBANWARFARE_API UWarfareAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UWarfareAnim();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UFUNCTION()
	bool CacheAndInit();

protected:
	UFUNCTION()
	void SetMovementProperty();
	UFUNCTION()
	void SetTransitionProperty();

protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> ThePlayer;

	UPROPERTY()
	TObjectPtr<class UPlayerBehaviorComponent> PlayerBehavior;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	float MovementSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	float MovementDirection = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	bool bCrouching = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	bool bWalking = false;
	
	bool bInitFlag = false;
	FTimerHandle InitTimer;
	
};
