// Fill out your copyright notice in the Description page of Project Settings.


#include "GunShotPlayer.h"
#include "Kismet/GameplayStatics.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/AttackComponent.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

void UGunShotPlayer::ExternalInitialize(APlayerBase* const InOwnerPawn)
{
	OwnerPawn = InOwnerPawn;
	if (OwnerPawn)
		World = OwnerPawn->GetWorld();
	else
	{
		LOG_EFUNC(TEXT("Initialization failed: OwnerPawn."));
	}

	if (World)
		World->GetTimerManager().SetTimer(InitTimer, this, &UGunShotPlayer::InitConstruct, 0.1f, true);
	else
	{
		LOG_EFUNC(TEXT("Initialization failed: World."));
	}
}

void UGunShotPlayer::BeginDestroy()
{
	Super::BeginDestroy();

	if (World)
		World->GetTimerManager().ClearAllTimersForObject(this);
}

void UGunShotPlayer::InitConstruct()
{
	if (InitCount < 0)
	{
		LOG_EFUNC(TEXT("Initialization failed: Unable to initialize. timer clear."));
		World->GetTimerManager().ClearTimer(InitTimer);
		return;
	}

	WeaponComponent = OwnerPawn->GetWeaponComponent();
	if (!WeaponComponent)
	{
		LOG_EFUNC(TEXT("Initialization failed: WeaponComponent. Retry:%d"), InitCount--);
		return;
	}

	AttackComponent = OwnerPawn->GetAttackComponent();
	if (!AttackComponent)
	{
		LOG_EFUNC(TEXT("Initialization failed: AttackComponent. Retry:%d"), InitCount--);
		return;
	}

	LOG_EFUNC(TEXT("The object successfully initialized."));
	bIsInitialized = true;
	World->GetTimerManager().ClearTimer(InitTimer);

	WeaponComponent->OnWeaponChange.BindUObject(this, &UGunShotPlayer::OnWeaponChange);
	AttackComponent->OnFire.BindUObject(this, &UGunShotPlayer::PlayGunShotSound);
}

void UGunShotPlayer::PlayGunShotSound()
{
	UGameplayStatics::PlaySoundAtLocation(World, GunShotSound, OwnerPawn->GetActorLocation());
}

void UGunShotPlayer::OnWeaponChange()
{
	uint8 WeaponId = WeaponComponent->GetEquippedWeaponId();
	UWeaponDataAsset* CurrentWeaponData = World->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(WeaponId);

	GunShotSound = CurrentWeaponData->SingleGunShotSound;
}
