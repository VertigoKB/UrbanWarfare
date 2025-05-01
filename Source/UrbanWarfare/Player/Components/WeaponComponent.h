 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "UrbanWarfare/Common/CommonStructs.h"
#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"
#include "SerializedWeaponInventory.h"
#include "WeaponComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnInventoryChange, const TArray<FInventoryItem>&)
DECLARE_DELEGATE_OneParam(FOnLocalPlayerEquipWeapon, EWeaponType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponChange, uint8)

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
	void LootWeapon(const FDroppedWeaponData& InData);

	// Call by ReloadState with authority.
	void Client_OnCompleteReload();

	UFUNCTION(Server, Reliable)
	void Server_RequestReloadAmmo(uint16 InRemainAmmo);

	UFUNCTION(Client, Reliable)
	void Client_ApplyReloadAmmo(uint16 InMag, uint16 InExtra);

	EWeaponType GetEquippedWeaponType() const;
	uint8 GetEquippedWeaponId() const;
	class UAmmoHandler* GetAmmoHandler() const;
	inline float GetCurrentHeadDamage() const { return CurrentHeadDamage; }
	inline float GetCurrentBodyDamage() const { return CurrentBodyDamage; }
	inline float GetCurrentAttackInterval() const { return AttackInterval; }
	inline float GetVerticalRecoil() const { return VerticalRecoil; }
	inline float GetHorizontalRecoil() const { return HorizontalRecoil; }

private:

	bool InitConstruct();

	UFUNCTION()
	void OnRep_WeaponInventory();

	UFUNCTION()
	void OnRep_EquippedWeaponId();

	UFUNCTION()
	void OnRep_EquippedWeaponType();
	
	//UFUNCTION()
	//void OnRep_EquippedWeaponType();
	UFUNCTION(Server, Reliable)
	void Server_RequestModifyAmmo(const uint8 Index, const uint16 InMag, const uint16 InExtra);
	
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon(const uint8 InIdNumber, const EWeaponType InType);

	void OnTriggeredEquipRifle();
	UFUNCTION(Server, Unreliable)
	void Server_ExecuteEquipRifle();

	void OnTriggeredEquipPistol();
	UFUNCTION(Server, Unreliable)
	void Server_ExecuteEquipPistol();

	void Client_OnTriggeredThrowWeapon();
	UFUNCTION(Server, Reliable)
	void Server_ExecuteThrowWeapon(uint16 InMag, uint16 InExtra);

	void OnTriggeredReload();

	UFUNCTION(Server, Reliable)
	void Server_PlayReloadMontage();


	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReloadWeapon(EWeaponType InType);

public:
	FOnInventoryChange OnInventoryChange;
	FOnLocalPlayerEquipWeapon OnLocalPlayerEquipWeapon;
	FOnWeaponChange OnWeaponChange;

protected:
	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;

	//UPROPERTY()
	//TObjectPtr<class UWarfareAnim> OwnerTheMeshAnim;

	//UPROPERTY()
	//TObjectPtr<class UWarfareAnim> OwnerThirdMeshAnim;

	UPROPERTY()
	TObjectPtr<class UAmmoHandler> AmmoHandler;

	bool bIsTriggeredReload = false;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeaponType)
	EWeaponType EquippedWeaponType = EWeaponType::UnArmed;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeaponId)
	uint8 EquippedWeaponId = 0;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponInventory)
	FWeaponInventory WeaponInventory;

	bool bCanLootWeapon = true;

private:

	float CurrentHeadDamage = 0.f;
	float CurrentBodyDamage = 0.f;
	float AttackInterval = 0.f;
	float HorizontalRecoil = 0.f;
	float VerticalRecoil = 0.f;


public:
	//// Debug
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool DebugBoolA = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool DebugBoolB = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool DebugBoolC = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//EWeaponType DebugWeaponType = EWeaponType::UnArmed;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint8 DebugWeaponIdA = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DebugAmmo = 0;
};
