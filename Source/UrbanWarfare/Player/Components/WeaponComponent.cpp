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

	FInventoryItem InitItem;
	InitItem.WeaponId = 0;
	WeaponInventory.Items.Init(InitItem, 4);
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

	DOREPLIFETIME(UWeaponComponent, WeaponInventory);
	DOREPLIFETIME(UWeaponComponent, EquippedWeaponType);
	DOREPLIFETIME(UWeaponComponent, EquippedWeaponId);
}

bool UWeaponComponent::IsPlayerHaveThisWeaponType(const EWeaponType InType) const
{
	if (WeaponInventory.Items[static_cast<uint8>(InType)].WeaponId != 0)
		return true;
	else
		return false;
}

void UWeaponComponent::LootWeapon(const uint8 InWeaponIdNumber, const EWeaponType InType)
{
	FInventoryItem Looted;
	Looted.WeaponId = InWeaponIdNumber;
	WeaponInventory.AddItem(Looted);

	if (EquippedWeaponId == 0)
		ServerEquipWeapon(InWeaponIdNumber, InType);
}

EWeaponType UWeaponComponent::GetEquippedWeaponType() const { return EquippedWeaponType; }

void UWeaponComponent::OnRep_WeaponInventory()
{

}

void UWeaponComponent::OnRep_EquippedWeaponId()
{
	UWeaponDataAsset* TempWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(EquippedWeaponId);

	if (TempWeaponData)
	{
		switch (TempWeaponData->WeaponType)
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
	else // 잘못된 무기 데이터 요청
		ensure(false);
}

void UWeaponComponent::ServerEquipWeapon_Implementation(const uint8 InIdNumber, const EWeaponType InType)
{
	EquippedWeaponId = InIdNumber;
	EquippedWeaponType = InType;

	OnRep_EquippedWeaponId();
}

// https://chatgpt.com/share/67ef9b17-750c-8010-8005-a206185355d3