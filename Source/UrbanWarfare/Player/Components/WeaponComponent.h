 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UrbanWarfare/Common/CommonEnums.h"
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

	inline EWeaponType GetEquippedWeaponType() const { return EquippedWeapon.Type; }

private:
	void LootWeapon(const EWeaponItem InWeapon);

	UFUNCTION()
	void OnRep_WeaponInventory();

	void EquipWeaponFromInventory(uint8 InIndex);

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//EWeaponType EquippedWeaponType = EWeaponType::UnArmed;

	UPROPERTY()
	FInventoryItem EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponInventory)
	FWeaponInventory WeaponInventory;

	bool bCanLootWeapon = true;

	friend class AWeaponBase;
};
