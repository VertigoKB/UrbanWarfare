// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerBase.h"
#include "Components/PlayerBehaviorComponent.h"

#include "UrbanWarfare/Common/CommonEnums.h"
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

	void PlayMontage_ReloadPistol();
	void PlayMontage_ReloadRifle();

protected:
	UFUNCTION()
	bool CacheAndInit();

protected:
	UFUNCTION()
	void SetMovementProperty();
	UFUNCTION()
	void SetTransitionProperty();
	UFUNCTION()
	void UpdateAimDirection();

protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> ThePlayer;

	UPROPERTY()
	TObjectPtr<class UPlayerBehaviorComponent> PlayerBehavior;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageReloadPistol;
	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageReloadRifle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	float MovementSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	float MovementDirection = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	bool bInAir = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	EMovementState CurrentMovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	EWeaponType EquippedWeapon = EWeaponType::UnArmed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UrbanWarfare")
	float AimDirection = 0.f;

	bool bInitFlag = false;
	FTimerHandle InitTimer;
	
};
