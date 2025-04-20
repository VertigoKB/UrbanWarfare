// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/RegisterInputComponent.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UAttackComponent, bAutoModeEffectFlag);
	DOREPLIFETIME(UAttackComponent, bSingleModeEffectFlag);
}

void UAttackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	bInitFlag = InitConsruct();
	if (!bInitFlag)
	{
		GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([this]() {

			bInitFlag = InitConsruct();
			if (bInitFlag)
			{
				OnInitializationComplete();
				GetWorld()->GetTimerManager().ClearTimer(InitTimer);
			}
			else
			{
				if (InitCount > 0)
					InitCount--;
				else
					GetWorld()->GetTimerManager().ClearTimer(InitTimer);
			}

			}), 0.2f, true);
	}
	else
		OnInitializationComplete();
	
}

bool UAttackComponent::InitConsruct()
{
	OwnerPawn = GetOwner<APlayerBase>();
	if (!OwnerPawn)
		return false;

	OwnerPlayerController = GetWorld()->GetFirstPlayerController();
	if (!OwnerPlayerController)
		return false;

	RegisterInputComponent = OwnerPawn->GetRegInputComp();
	if (!RegisterInputComponent)
		return false;

	WeaponComponent = OwnerPawn->GetWeaponComponent();
	if (!WeaponComponent)
		return false;

	WeaponPreLoader = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>();
	if (!WeaponPreLoader)
		return false;

	return true;
}

void UAttackComponent::OnInitializationComplete()
{
	RegisterInputComponent->OnAttack.BindUObject(this, &UAttackComponent::Server_TriggerAttack);
	WeaponComponent->OnWeaponChange.BindUObject(this, &UAttackComponent::OnWeaponChange);
}

void UAttackComponent::Server_TriggerAttack_Implementation()
{
	uint8 EquippedWeaponNumber = WeaponComponent->GetEquippedWeaponId();
	UWeaponDataAsset* EquippedWeaponData = WeaponPreLoader->GetWeaponDataByWeaponId(EquippedWeaponNumber);

	bIsAttackTriggered = !bIsAttackTriggered;

	switch (EquippedWeaponData->FiringMode)
	{
	case EFiringMode::Single:

		if (bIsAttackTriggered && bSingleAttackActive)
		{
			bSingleAttackActive = false;

			AttackInterval = WeaponComponent->GetCurrentAttackInterval();
			Damage = WeaponComponent->GetCurrentDamage();

			AttackLineTrace();

			bSingleModeEffectFlag = !bSingleModeEffectFlag;
			OnRep_bSingleModeEffectFlag();

			GetWorld()->GetTimerManager().SetTimer(RoundIntervalHandle, FTimerDelegate::CreateLambda([this]() {
				bSingleAttackActive = true;
				}), AttackInterval, false);
		}

		break;
	case EFiringMode::Auto:

		if (bIsAttackTriggered)
		{
			AttackInterval = WeaponComponent->GetCurrentAttackInterval();
			Damage = WeaponComponent->GetCurrentDamage();

			bAutoModeEffectFlag = true;
			OnRep_bAutoModeEffectFlag();
			GetWorld()->GetTimerManager().SetTimer(RoundIntervalHandle, this, &UAttackComponent::AttackLineTrace, AttackInterval, true, 0.f);
		}
		else
		{
			bAutoModeEffectFlag = false;
			OnRep_bAutoModeEffectFlag();
			GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);
		}

		break;
	case EFiringMode::Burst:
		break;
	case EFiringMode::Manual:
		break;
	}
	
}

void UAttackComponent::AttackLineTrace()
{
	FHitResult HitResult;
	//FVector StartLocation = OwnerPlayerController->PlayerCameraManager->GetCameraLocation();
	FVector StartLocation = OwnerPawn->GetPlayerCamera()->GetComponentLocation();
	FVector TargetLocation = OwnerPawn->GetPlayerCamera()->GetForwardVector() * 20000.f;
	FVector EndLocation = StartLocation + TargetLocation;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, CollisionParams);

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.f);
	if (bHit)
	{
		FString TargetActor = *HitResult.GetActor()->GetActorNameOrLabel();
		//LOG_SIMPLE(TEXT("Test: hit Actor is %s"), *TargetActor);
	}
}

void UAttackComponent::OnWeaponChange()
{
	if (GetOwner()->HasAuthority() &&
		bIsAttackTriggered)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(RoundIntervalHandle))
			GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);

		AttackInterval = WeaponComponent->GetCurrentAttackInterval();
		Damage = WeaponComponent->GetCurrentDamage();
		GetWorld()->GetTimerManager().SetTimer(RoundIntervalHandle, this, &UAttackComponent::AttackLineTrace, AttackInterval, true, 0.f);
	}
}

void UAttackComponent::OnRep_bAutoModeEffectFlag()
{
	if (!bInitFlag)
		return;

	if (bAutoModeEffectFlag)
	{
		uint8 EquippedWeaponNumber = WeaponComponent->GetEquippedWeaponId();
		UWeaponDataAsset* EquippedWeaponData = WeaponPreLoader->GetWeaponDataByWeaponId(EquippedWeaponNumber);

		ParticleToPlay = EquippedWeaponData->MuzzleFlash;
		ComponentToPlay = OwnerPawn->GetWeaponMesh();

		AttackInterval = WeaponComponent->GetCurrentAttackInterval();
		GetWorld()->GetTimerManager().SetTimer(EffectHandle, FTimerDelegate::CreateLambda([this]() {

			if (ComponentToPlay->SkeletalMesh)
			{
				UGameplayStatics::SpawnEmitterAttached(ParticleToPlay, ComponentToPlay, FName("MuzzleSocket"),
					FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true,
					EPSCPoolMethod::AutoRelease);
			}
			
			}), AttackInterval, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(EffectHandle);
	}
}

void UAttackComponent::OnRep_bSingleModeEffectFlag()
{
	if (!bInitFlag)
		return;

	uint8 EquippedWeaponNumber = WeaponComponent->GetEquippedWeaponId();
	UWeaponDataAsset* EquippedWeaponData = WeaponPreLoader->GetWeaponDataByWeaponId(EquippedWeaponNumber);

	ParticleToPlay = EquippedWeaponData->MuzzleFlash;
	ComponentToPlay = OwnerPawn->GetWeaponMesh();

	if (ComponentToPlay->SkeletalMesh)
	{
		UGameplayStatics::SpawnEmitterAttached(ParticleToPlay, ComponentToPlay, FName("MuzzleSocket"),
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true,
			EPSCPoolMethod::AutoRelease);
	}
}

// Called every frame
//void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

