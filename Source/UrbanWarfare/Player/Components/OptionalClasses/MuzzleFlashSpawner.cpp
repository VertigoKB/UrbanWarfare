// Fill out your copyright notice in the Description page of Project Settings.


#include "MuzzleFlashSpawner.h"
#include "Kismet/GameplayStatics.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"

void UMuzzleFlashSpawner::ExternalInitialize(APlayerBase* const InRootOwner)
{
	OwnerPawn = InRootOwner;
	World = OwnerPawn->GetWorld();
	OwnerPawn->GetWeaponComponent()->OnWeaponChange.AddUObject(this, &UMuzzleFlashSpawner::SetTargetComponent);
}

void UMuzzleFlashSpawner::SetTargetComponent(uint8 InWeaponId)
{
	UWeaponDataAsset* EquippedWeaponData = World->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(InWeaponId);

	ParticleToPlay = EquippedWeaponData->MuzzleFlash;
	ComponentToPlay = OwnerPawn->GetWeaponMesh();
}

void UMuzzleFlashSpawner::PlayMuzzleEffect()
{
	if (!ComponentToPlay || !ParticleToPlay)
		SetTargetComponent(OwnerPawn->GetWeaponComponent()->GetEquippedWeaponId());

	UGameplayStatics::SpawnEmitterAttached(ParticleToPlay, ComponentToPlay, FName("MuzzleSocket"),
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true,
		EPSCPoolMethod::AutoRelease);
}

