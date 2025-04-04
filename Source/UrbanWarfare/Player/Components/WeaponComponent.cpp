// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Misc/CoreMiscDefines.h"

#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Weapon/WeaponBase.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//WeaponInventory.Reserve(4);

	EquippedWeapon.Type = EWeaponType::UnArmed;
	EquippedWeapon.Item = EWeaponItem::None;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, WeaponInventory);
}

bool UWeaponComponent::IsPlayerHaveThisWeaponType(const EWeaponType InType) const
{
	//check(false);

	//ensure(false);

	for (const auto Iter : WeaponInventory.Items)
	{
		if (Iter.Type == InType)
			return true;
	}

	return false;
}

// 이 함수는 AWeaponBase에 의해 HasAuthority true 확인후 호출됨
void UWeaponComponent::LootWeapon(const EWeaponItem InWeapon)
{
	UBlueprintConfig* BlueprintConfig = GetOwner<APlayerBase>()->GetBlueprintConfig();

	FInventoryItem TargetItem;

	switch (InWeapon)
	{
	case EWeaponItem::None:
		break;
	case EWeaponItem::Pistol:
		TargetItem.Item = EWeaponItem::Pistol;
		TargetItem.Type = EWeaponType::Pistol;
		break;
	case EWeaponItem::AK47:
		TargetItem.Item = EWeaponItem::AK47;
		TargetItem.Type = EWeaponType::Rifle;
		break;
	case EWeaponItem::M4A1:
		TargetItem.Item = EWeaponItem::M4A1;
		TargetItem.Type = EWeaponType::Rifle;
		break;
	}

	WeaponInventory.AddItem(TargetItem);

	if (GetOwner<APawn>()->IsLocallyControlled())
		OnRep_WeaponInventory();
}

void UWeaponComponent::OnRep_WeaponInventory()
{
	// 테스트 코드
	for (const auto& Iter : WeaponInventory.Items)
	{
		uint8 TypeIndex = static_cast<uint8>(Iter.Type);
		uint8 ItemIndex = static_cast<uint8>(Iter.Item);
	
		UE_LOG(LogTemp, Warning, TEXT("[UrbanWarfare] Type: %d, Item: %d"), TypeIndex, ItemIndex);
	}
}

void UWeaponComponent::EquipWeaponFromInventory(uint8 InIndex)
{
	EquippedWeapon.Type = WeaponInventory.Items[InIndex].Type;
	EquippedWeapon.Item = WeaponInventory.Items[InIndex].Item;
}
