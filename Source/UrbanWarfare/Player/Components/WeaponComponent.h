 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"
#include "SerializedWeaponInventory.h"
#include "WeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsPlayerHaveThisWeaponType(const EWeaponType InType) const;

	// 이 함수는 ADroppedWeapon에 의해 HasAuthority true 확인후 호출됨
	void LootWeapon(const uint8 InWeaponIdNumber, const EWeaponType InType);

	EWeaponType GetEquippedWeaponType() const;

private:

	bool InitConstruct();

	UFUNCTION()
	void OnRep_WeaponInventory();

	UFUNCTION()
	void OnRep_EquippedWeaponId();

	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(const uint8 InIdNumber, const EWeaponType InType);

protected:
	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;

	UPROPERTY(Replicated)
	EWeaponType EquippedWeaponType = EWeaponType::UnArmed;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeaponId)
	uint8 EquippedWeaponId = 0;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponInventory)
	FWeaponInventory WeaponInventory;

	bool bCanLootWeapon = true;

public:
	// Debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugBoolA = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugBoolB = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugBoolC = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType DebugWeaponType = EWeaponType::UnArmed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DebugWeaponIdA = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DebugWeaponIdB = 0;
};
