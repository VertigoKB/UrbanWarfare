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
	void LootWeapon(const uint8 InWeaponIdNumber);

	inline EWeaponType GetEquippedWeaponType() const { return EquippedWeaponType; }

private:

	//UFUNCTION()
	//void OnRep_WeaponInventory();

	UFUNCTION()
	void OnRep_LootTargetId();

	UFUNCTION(Server, Unreliable)
	void ResetLootTargetId();

	void EquipWeaponFromInventory(const EWeaponType InType);

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	EWeaponType EquippedWeaponType = EWeaponType::UnArmed;

	UPROPERTY(Replicated)
	uint8 EquippedWeaponId = 0;

	//UPROPERTY()
	//TObjectPtr<UWeaponDataAsset> EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_LootTargetId)
	uint8 LootTargetId = 0;

	UPROPERTY()
	TMap<EWeaponType, uint8> WeaponInventory;


	//UPROPERTY(ReplicatedUsing = OnRep_WeaponInventory)
	//FWeaponInventory WeaponInventory;

	bool bCanLootWeapon = true;

	//friend class AWeaponBase;
};
