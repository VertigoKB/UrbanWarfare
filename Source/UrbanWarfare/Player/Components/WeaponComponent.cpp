// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Misc/CoreMiscDefines.h"
#include "Components/SkeletalMeshComponent.h"


#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/WarfareAnim.h"
#include "UrbanWarfare/Player/Components/RegisterInputComponent.h"
#include "UrbanWarfare/Player/Components/OptionalClasses/AmmoHandler.h"
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
		RegisterInputComponent->OnInputEquipRifle.BindUObject(this, &UWeaponComponent::OnTriggeredEquipRifle);
		RegisterInputComponent->OnInputEquipPistol.BindUObject(this, &UWeaponComponent::OnTriggeredEquipPistol);
		RegisterInputComponent->OnThrowWeapon.BindUObject(this, &UWeaponComponent::Client_OnTriggeredThrowWeapon);
		RegisterInputComponent->OnInputReload.BindUObject(this, &UWeaponComponent::OnTriggeredReload);

		AmmoHandler = NewObject<UAmmoHandler>();
		AmmoHandler->ExternalInitialize(GetOwner<APlayerBase>(), this);
	}

	if (GetOwner()->HasAuthority())
	{
		FInventoryItem InitItem;
		InitItem.WeaponId = 0;
		//InitItem.ReplicationID = GenerateReplicationID();
		WeaponInventory.Items.Init(InitItem, 4);
		WeaponInventory.MarkArrayDirty();
	}
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

void UWeaponComponent::LootWeapon(const FDroppedWeaponData& InData)
{
	WeaponInventory.SetItem(static_cast<uint8>(InData.WeaponType), InData.WeaponId, InData.AmmoInMag, InData.ExtraAmmo);
	OnRep_WeaponInventory();

	if (EquippedWeaponId == 0)
		Server_EquipWeapon(InData.WeaponId, InData.WeaponType);
}

void UWeaponComponent::Client_OnCompleteReload()
{
	if (bIsTriggeredReload)
	{
		bIsTriggeredReload = false;

		FWeaponAmmoData CurrentAmmoData = AmmoHandler->GetAmmoData();
		uint16 RemainMag = CurrentAmmoData.AmmoInMag;
		uint16 RemainExtra = CurrentAmmoData.ExtraAmmo;

		UWeaponDataAsset* CurrentWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(EquippedWeaponId);
		if (CurrentWeaponData)
		{
			uint16 LoadableInMag = CurrentWeaponData->LoadableAmmoPerMag;

			uint16 RequiredAmount = LoadableInMag - RemainMag;

			uint16 NewInMag = 0;
			uint16 NewInExtra = 0;
			if (RequiredAmount < RemainExtra)
			{
				NewInMag = RemainMag + RequiredAmount;
				NewInExtra = RemainExtra - RequiredAmount;
			}
			else
				NewInMag = RemainMag + RemainExtra;

			FWeaponAmmoData NewAmmoData;
			NewAmmoData.AmmoInMag = NewInMag;
			NewAmmoData.ExtraAmmo = NewInExtra;

			AmmoHandler->SetAmmoData(NewAmmoData);
			Server_RequestModifyAmmo(static_cast<uint8>(EquippedWeaponType), NewInMag, NewInExtra);
		}
	}
}

void UWeaponComponent::Server_RequestReloadAmmo_Implementation(uint16 InRemainAmmo)
{
	//WeaponInventory.Items[static_cast<uint8>(EquippedWeaponType)].AmmoInMag
	
}

void UWeaponComponent::Client_ApplyReloadAmmo_Implementation(uint16 InMag, uint16 InExtra)
{

}

void UWeaponComponent::Server_RequestModifyAmmo_Implementation(const uint8 Index, const uint16 InAmmo, const uint16 InExtra)
{
	WeaponInventory.ModifyAmmo(Index, InAmmo, InExtra);
}

EWeaponType UWeaponComponent::GetEquippedWeaponType() const { return EquippedWeaponType; }
uint8 UWeaponComponent::GetEquippedWeaponId() const { return EquippedWeaponId; }
UAmmoHandler* UWeaponComponent::GetAmmoHandler() const { return AmmoHandler; }

bool UWeaponComponent::InitConstruct()
{
	bool bIsSuccess = false;

	RegisterInputComponent = GetOwner<APlayerBase>()->GetRegInputComp();
	if (!RegisterInputComponent)
	{
		LOG_EFUNC(TEXT("Cach failed: RegisterInputComponent"));
	}
	else
		bIsSuccess = true;

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
		GetOwner<APlayerBase>()->GetWeaponMesh()->SetVisibility(false);
		return;
	}

	UWeaponDataAsset* TempWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(EquippedWeaponId);
	AttackInterval = TempWeaponData->RoundInterval;
	CurrentHeadDamage = TempWeaponData->HeadDamage;
	CurrentBodyDamage = TempWeaponData->BodyDamage;
	HorizontalRecoil = TempWeaponData->HorizontalRecoil;
	VerticalRecoil = TempWeaponData->VerticalRecoil;
	OnWeaponChange.Broadcast(EquippedWeaponId);

	if (TempWeaponData)
	{
		switch (TempWeaponData->WeaponType)
		{
		case EWeaponType::UnArmed:
			GetOwner<APlayerBase>()->GetWeaponMesh()->SetVisibility(false);
			break;
		case EWeaponType::Rifle:
			GetOwner<APlayerBase>()->GetWeaponMesh()->SetVisibility(true);
			GetOwner<APlayerBase>()->GetWeaponMesh()->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
			GetOwner<APlayerBase>()->AttachWeaponMeshToRifle();
			break;
		case EWeaponType::Pistol:
			GetOwner<APlayerBase>()->GetWeaponMesh()->SetVisibility(true);
			GetOwner<APlayerBase>()->GetWeaponMesh()->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
			GetOwner<APlayerBase>()->AttachWeaponMeshToPistol();
			break;
		}
	}
	else // 잘못된 무기 데이터 요청
		ensure(false);
}

void UWeaponComponent::OnRep_EquippedWeaponType()
{
	OnLocalPlayerEquipWeapon.ExecuteIfBound(EquippedWeaponType);

	FName WhoIsThis = GetOwner()->GetFName();
	ENetMode WatMode = GetWorld()->GetNetMode();

	if (GetOwner<APlayerBase>()->IsLocallyControlled())
	{
		uint8 InventoryAmmoInMag = WeaponInventory.Items[static_cast<uint8>(EquippedWeaponType)].AmmoInMag;
		uint8 InventoryExtraAmmo = WeaponInventory.Items[static_cast<uint8>(EquippedWeaponType)].ExtraAmmo;

		FWeaponAmmoData AmmoData;
		AmmoData.AmmoInMag = InventoryAmmoInMag;
		AmmoData.ExtraAmmo = InventoryExtraAmmo;

		AmmoHandler->RefreshAmmoHandler(AmmoData, EquippedWeaponType);
	}
}

void UWeaponComponent::Server_EquipWeapon_Implementation(const uint8 InIdNumber, const EWeaponType InType)
{
	EquippedWeaponId = InIdNumber;
	EquippedWeaponType = InType;

	OnRep_EquippedWeaponId();
	OnRep_EquippedWeaponType();
	Multicast_ReloadWeapon(InType);
}

void UWeaponComponent::OnTriggeredEquipRifle()
{
	bIsTriggeredReload = false;
	FWeaponAmmoData AmmoData = AmmoHandler->GetAmmoData();
	uint16 ReaminAmmoInMag = AmmoData.AmmoInMag;
	uint16 RemainExtraAmmo = AmmoData.ExtraAmmo;

	Server_RequestModifyAmmo(static_cast<uint8>(EquippedWeaponType), ReaminAmmoInMag, RemainExtraAmmo);
	Server_ExecuteEquipRifle();
}

void UWeaponComponent::Server_ExecuteEquipRifle_Implementation()
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

void UWeaponComponent::OnTriggeredEquipPistol()
{
	bIsTriggeredReload = false;
	FWeaponAmmoData AmmoData = AmmoHandler->GetAmmoData();
	uint16 ReaminAmmoInMag = AmmoData.AmmoInMag;
	uint16 RemainExtraAmmo = AmmoData.ExtraAmmo;

	Server_RequestModifyAmmo(static_cast<uint8>(EquippedWeaponType), ReaminAmmoInMag, RemainExtraAmmo);
	Server_ExecuteEquipPistol();
}

void UWeaponComponent::Server_ExecuteEquipPistol_Implementation()
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

void UWeaponComponent::Client_OnTriggeredThrowWeapon()
{
	bIsTriggeredReload = false;
	FWeaponAmmoData AmmoData = AmmoHandler->GetAmmoData();
	Server_ExecuteThrowWeapon(AmmoData.AmmoInMag, AmmoData.ExtraAmmo);
}

void UWeaponComponent::Server_ExecuteThrowWeapon_Implementation(uint16 InMag, uint16 InExtra)
{
	if (EquippedWeaponId == 0)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APlayerBase* MyOwnerPawn = GetOwner<APlayerBase>();
	
	FVector SpawnLocation = MyOwnerPawn->GetActorLocation() + FVector(0.f, 0.f, 50.f);
	FRotator SpawnRotation = MyOwnerPawn->GetActorRotation();
	ADroppedWeapon* DroppedWeapon = GetWorld()->SpawnActor<ADroppedWeapon>(ADroppedWeapon::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	FWeaponAmmoData InitAmmoData;
	InitAmmoData.AmmoInMag = InMag;
	InitAmmoData.ExtraAmmo = InExtra;
	DroppedWeapon->ExternalInitialize(EquippedWeaponId, InitAmmoData);

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
		Multicast_ReloadWeapon(EquippedWeaponType);
	}
	else
	{
		EquippedWeaponId = 0;
		EquippedWeaponType = EWeaponType::UnArmed;

		OnRep_EquippedWeaponId();
		OnRep_EquippedWeaponType();
	}
}

void UWeaponComponent::OnTriggeredReload()
{
	if (!bIsTriggeredReload)
		Server_PlayReloadMontage();
	bIsTriggeredReload = true;
}

void UWeaponComponent::Server_PlayReloadMontage_Implementation()
{
	Multicast_ReloadWeapon(EquippedWeaponType);
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
