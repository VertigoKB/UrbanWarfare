// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Misc/CoreMiscDefines.h"
#include "Components/SkeletalMeshComponent.h"


#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/WarfareAnim.h"
#include "UrbanWarfare/Player/Components/RegisterInputComponent.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Weapon/DropeedWeapon.h"
#include "UrbanWarfare/Common/WarfareLogger.h"
//#include "UrbanWarfare/Weapon/WeaponBase.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (InitConstruct())
	{
		RegisterInputComponent->OnInputEquipRifle.BindUObject(this, &UWeaponComponent::Server_OnTriggerEquipRifle);
		RegisterInputComponent->OnInputEquipPistol.BindUObject(this, &UWeaponComponent::Server_OnTriggerEquipPistol);
		RegisterInputComponent->OnThrowWeapon.BindUObject(this, &UWeaponComponent::Server_OnTriggerThrowWeapon);
	}

	FInventoryItem InitItem;
	InitItem.WeaponId = 0;
	WeaponInventory.Items.Init(InitItem, 4);
	WeaponInventory.MarkArrayDirty();
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
	WeaponInventory.SetItem(static_cast<uint8>(InType), InWeaponIdNumber);
	OnRep_WeaponInventory();

	if (EquippedWeaponId == 0)
		Server_EquipWeapon(InWeaponIdNumber, InType);
}

EWeaponType UWeaponComponent::GetEquippedWeaponType() const { return EquippedWeaponType; }

bool UWeaponComponent::InitConstruct()
{
	bool bIsSuccess = false;

	switch (true)
	{
	case true:
	default:
		RegisterInputComponent = GetOwner<APlayerBase>()->GetRegInputComp();
		if (!RegisterInputComponent)
		{
			LOG_EFUNC(TEXT("Cach failed: RegisterInputComponent"));
			break;
		}

		OwnerTheMeshAnim = Cast<UWarfareAnim>(GetOwner<APlayerBase>()->GetTheMesh()->GetAnimInstance());
		if (!OwnerTheMeshAnim)
		{
			LOG_EFUNC(TEXT("Cach failed: OwnerTheMeshAnim"));
			break;
		}

		OwnerThirdMeshAnim = Cast<UWarfareAnim>(GetOwner<APlayerBase>()->GetThirdMesh()->GetAnimInstance());
		if (!OwnerThirdMeshAnim)
		{
			LOG_EFUNC(TEXT("Cach failed: OwnerThirdMeshAnim"));
			break;
		}
		
		bIsSuccess = true;
		break;
	}

	return bIsSuccess;
}

void UWeaponComponent::OnRep_WeaponInventory()
{
	OnInventoryChange.ExecuteIfBound(WeaponInventory.Items);
}

void UWeaponComponent::OnRep_EquippedWeaponId()
{
	if (EquippedWeaponId == 0)
	{
		GetOwner<APlayerBase>()->GetRifleMesh()->SetSkeletalMesh(nullptr);
		GetOwner<APlayerBase>()->GetPistolMesh()->SetSkeletalMesh(nullptr);
		return;
	}

	UWeaponDataAsset* TempWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(EquippedWeaponId);

	if (TempWeaponData)
	{
		switch (TempWeaponData->WeaponType)
		{
		case EWeaponType::UnArmed:
			GetOwner<APlayerBase>()->GetRifleMesh()->SetSkeletalMesh(nullptr);
			GetOwner<APlayerBase>()->GetPistolMesh()->SetSkeletalMesh(nullptr);
			break;
		case EWeaponType::Rifle:
			GetOwner<APlayerBase>()->GetRifleMesh()->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
			GetOwner<APlayerBase>()->GetPistolMesh()->SetSkeletalMesh(nullptr);
			break;
		case EWeaponType::Pistol:
			GetOwner<APlayerBase>()->GetRifleMesh()->SetSkeletalMesh(nullptr);
			GetOwner<APlayerBase>()->GetPistolMesh()->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
			break;
		}
	}
	else // 잘못된 무기 데이터 요청
		ensure(false);
}

void UWeaponComponent::OnRep_EquippedWeaponType()
{
	OnLocalPlayerEquipWeapon.ExecuteIfBound(EquippedWeaponType);

}

void UWeaponComponent::Server_EquipWeapon_Implementation(const uint8 InIdNumber, const EWeaponType InType)
{
	EquippedWeaponId = InIdNumber;
	EquippedWeaponType = InType;

	OnRep_EquippedWeaponId();
	OnRep_EquippedWeaponType();
}

void UWeaponComponent::Server_OnTriggerEquipRifle_Implementation()
{
	uint8 TargetWeapon = WeaponInventory.Items[static_cast<uint8>(EWeaponType::Rifle)].WeaponId;

	if (TargetWeapon != 0 && TargetWeapon != EquippedWeaponId)
	{
		EquippedWeaponId = TargetWeapon;
		EquippedWeaponType = EWeaponType::Rifle;

		OnRep_EquippedWeaponId();
		OnRep_EquippedWeaponType();
		Multicast_ReloadWeapon(EWeaponType::Rifle);
	}
}

void UWeaponComponent::Server_OnTriggerEquipPistol_Implementation()
{
	uint8 TargetWeapon = WeaponInventory.Items[static_cast<uint8>(EWeaponType::Pistol)].WeaponId;

	if (TargetWeapon != 0 && TargetWeapon != EquippedWeaponId)
	{
		EquippedWeaponId = TargetWeapon;
		EquippedWeaponType = EWeaponType::Pistol;

		OnRep_EquippedWeaponId();
		OnRep_EquippedWeaponType();
		Multicast_ReloadWeapon(EWeaponType::Pistol);
	}
}

void UWeaponComponent::Server_OnTriggerThrowWeapon_Implementation()
{
	if (EquippedWeaponId == 0)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APlayerBase* MyOwnerPawn = GetOwner<APlayerBase>();
	
	FVector SpawnLocation = MyOwnerPawn->GetActorLocation() + FVector(0.f, 0.f, 50.f);
	FRotator SpawnRotation = MyOwnerPawn->GetActorRotation();
	ADroppedWeapon* DroppedWeapon = GetWorld()->SpawnActor<ADroppedWeapon>(ADroppedWeapon::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	DroppedWeapon->ExternalInitialize(EquippedWeaponId);

	FVector ForwardVector = MyOwnerPawn->GetActorForwardVector();
	FVector UpVector = FVector::UpVector;
	FVector ThrowPower = (ForwardVector + UpVector * 0.3f).GetSafeNormal() * 550.f;
	DroppedWeapon->GetWeaponMesh()->AddImpulse(ThrowPower);

	WeaponInventory.SetItem(static_cast<uint8>(EquippedWeaponType), 0);
	OnRep_WeaponInventory();

	
	uint8 AnotherWeaponInInventory = 0;
	for (const auto& Iter : WeaponInventory.Items)
	{
		if (Iter.WeaponId > 0)
		{
			AnotherWeaponInInventory = Iter.WeaponId;
			break;
		}
	}

	if (AnotherWeaponInInventory != 0)
	{
		UWeaponDataAsset* WeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(AnotherWeaponInInventory);
		EquippedWeaponId = WeaponData->WeaponIdNumber;
		EquippedWeaponType = WeaponData->WeaponType;

		OnRep_EquippedWeaponId();
		OnRep_EquippedWeaponType();

	}
	else
	{
		EquippedWeaponId = 0;
		EquippedWeaponType = EWeaponType::UnArmed;

		OnRep_EquippedWeaponId();
		OnRep_EquippedWeaponType();
	}
}

void UWeaponComponent::Multicast_ReloadWeapon_Implementation(EWeaponType InType)
{
	UAnimInstance* AnimInstanceTheMesh = GetOwner<APlayerBase>()->GetTheMesh()->GetAnimInstance();
	UAnimInstance* AnimInstanceTheThirdMesh = GetOwner<APlayerBase>()->GetThirdMesh()->GetAnimInstance();
	UWarfareAnim* WarfareAnimTheMesh = Cast<UWarfareAnim>(AnimInstanceTheMesh);
	UWarfareAnim* WarfareAnimTheThirdMesh = Cast<UWarfareAnim>(AnimInstanceTheThirdMesh);

	if (!(AnimInstanceTheMesh) || !(AnimInstanceTheThirdMesh) || !(WarfareAnimTheMesh) || !(WarfareAnimTheThirdMesh))
	{
		LOG_EFUNC(TEXT("AnimInstance Issue"))
	}

	switch (InType)
	{
	case EWeaponType::Rifle:
			WarfareAnimTheMesh->PlayMontage_ReloadRifle();
			WarfareAnimTheThirdMesh->PlayMontage_ReloadRifle();
		break;
	case EWeaponType::Pistol:
			WarfareAnimTheMesh->PlayMontage_ReloadPistol();
			WarfareAnimTheThirdMesh->PlayMontage_ReloadPistol();
		break;
	}
}
// https://chatgpt.com/share/67ef9b17-750c-8010-8005-a206185355d3