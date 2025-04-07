// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Misc/CoreMiscDefines.h"
#include "Components/SkeletalMeshComponent.h"


#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
//#include "UrbanWarfare/Weapon/WeaponBase.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//WeaponInventory.Reserve(4);

	WeaponInventory.Add(EWeaponType::Pistol, 0);
	WeaponInventory.Add(EWeaponType::Rifle, 0);
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
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

	DOREPLIFETIME(UWeaponComponent, LootTargetId);
	DOREPLIFETIME(UWeaponComponent, EquippedWeaponType);
	DOREPLIFETIME(UWeaponComponent, EquippedWeaponId);
}

bool UWeaponComponent::IsPlayerHaveThisWeaponType(const EWeaponType InType) const
{
	for (TMap<EWeaponType, uint8>::TConstIterator Iter(WeaponInventory); Iter; ++Iter)
	{
		if (Iter->Key == InType)
		{
			if (Iter->Value > 0)
				return true;
		}
	}

	return false;
}

void UWeaponComponent::LootWeapon(const uint8 InWeaponIdNumber)
{
	LootTargetId = InWeaponIdNumber;

	if (GetOwner<APawn>()->IsLocallyControlled())
		OnRep_LootTargetId();
}

//void UWeaponComponent::OnRep_WeaponInventory()
//{
//	// 테스트 코드
//	for (const auto& Iter : WeaponInventory.Items)
//	{
//		uint8 TypeIndex = static_cast<uint8>(Iter.WeaponData->WeaponType);
//		FName ItemName = Iter.WeaponData->WeaponName;
//	
//		UE_LOG(LogTemp, Warning, TEXT("[UrbanWarfare] Type: %d, Item: %s"), TypeIndex, *ItemName.ToString());
//	}
//}

void UWeaponComponent::OnRep_LootTargetId()
{
	if (LootTargetId != 0)
	{
		UWeaponDataAsset* TempWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(LootTargetId);
		WeaponInventory.Add(TempWeaponData->WeaponType, LootTargetId);
		ResetLootTargetId();

		if (EquippedWeaponType == EWeaponType::UnArmed)
			EquipWeaponFromInventory(TempWeaponData->WeaponType);
	}
}

void UWeaponComponent::ResetLootTargetId_Implementation()
{
	LootTargetId = 0;
}

void UWeaponComponent::EquipWeaponFromInventory(const EWeaponType InType)
{
	EquippedWeaponType = InType;

	uint8 WeaponId = *(WeaponInventory.Find(InType));

	UWeaponDataAsset* TempWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(WeaponId);

	switch (EquippedWeaponType)
	{
	case EWeaponType::UnArmed:
		GetOwner<APlayerBase>()->GetRifleMesh()->SetSkeletalMesh(nullptr);
		break;
	case EWeaponType::Rifle:
		GetOwner<APlayerBase>()->GetRifleMesh()->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
		break;
	case EWeaponType::Pistol:
		GetOwner<APlayerBase>()->GetPistolMesh()->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
		break;
	}

}



// https://chatgpt.com/share/67ef9b17-750c-8010-8005-a206185355d3