// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
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
				LOG_NETEFUNC(TEXT("Initialization successfully complete."));
				OnInitializationComplete();
				GetWorld()->GetTimerManager().ClearTimer(InitTimer);
			}
			else
			{
				if (InitCount > 0)
				{
					LOG_NETEFUNC(TEXT("Retry initialization. Count: %d"), InitCount--);
				}
				else
				{
					LOG_NETEFUNC(TEXT("Unable to initialize."));
					GetWorld()->GetTimerManager().ClearTimer(InitTimer);
				}
			}

			}), 0.1f, true);
	}
	
}

bool UAttackComponent::InitConsruct()
{
	bool IsAuth = GetOwner()->HasAuthority();
	bool IsLocal = GetOwner<APlayerBase>()->IsLocallyControlled();
	FString WhatName = GetOwner()->GetName();

	switch (true)
	{
	case true:
	default:
	{
		OwnerPawn = GetOwner<APlayerBase>();
		if (!OwnerPawn)
		{
			LOG_NETEFUNC(TEXT("Initialization failed: OwnerPawn"));
			break;
		}
		OwnerPlayerController = OwnerPawn->GetController<AWarfareController>();
		if (!OwnerPlayerController)
		{
			LOG_NETEFUNC(TEXT("Initialization failed: OwnerPlayerController"));
			break;
		}
		RegisterInputComponent = OwnerPawn->GetRegInputComp();
		if (!RegisterInputComponent)
		{
			LOG_NETEFUNC(TEXT("Initialization failed: RegisterInputComponent"));
			break;
		}
		WeaponComponent = OwnerPawn->GetWeaponComponent();
		if (!WeaponComponent)
		{
			LOG_NETEFUNC(TEXT("Initialization failed: WeaponComponent"));
			break;
		}

		return true;
		break;
	}
	}
	return false;
}

void UAttackComponent::OnInitializationComplete()
{
	RegisterInputComponent->OnAttack.BindUObject(this, &UAttackComponent::Server_TriggerAttack);
	WeaponComponent->OnWeaponChange.BindUObject(this, &UAttackComponent::OnWeaponChange);
}

void UAttackComponent::Server_TriggerAttack_Implementation()
{
	bIsTriggeredAttack = !bIsTriggeredAttack;

	switch (bIsTriggeredAttack)
	{
	case true:
		AttackInterval = WeaponComponent->GetCurrentAttackInterval();
		Damage = WeaponComponent->GetCurrentDamage();
		GetWorld()->GetTimerManager().SetTimer(RoundIntervalHandle, this, &UAttackComponent::AttackLineTrace, AttackInterval, true, 0.f);
		break;
	case false:
		GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);
		break;
	}
}

void UAttackComponent::AttackLineTrace()
{
	LOG_SIMPLE(TEXT("Action"));
	FHitResult HitResult;
	//FVector StartLocation = OwnerPlayerController->PlayerCameraManager->GetCameraLocation();
	FVector StartLocation = OwnerPawn->GetPlayerCamera()->GetComponentLocation();
	FVector TargetLocation = OwnerPawn->GetActorForwardVector() * 20000.f;
	FVector EndLocation = StartLocation + TargetLocation;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams);
	
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.f);
	if (bHit)
	{
		FString TargetActor = *HitResult.GetActor()->GetActorNameOrLabel();
		LOG_SIMPLE(TEXT("Test: hit Actor is %s"), *TargetActor);
	}
}

void UAttackComponent::OnWeaponChange()
{
	if (GetOwner()->HasAuthority() &&
		bIsTriggeredAttack)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(RoundIntervalHandle))
			GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);

		AttackInterval = WeaponComponent->GetCurrentAttackInterval();
		Damage = WeaponComponent->GetCurrentDamage();
		GetWorld()->GetTimerManager().SetTimer(RoundIntervalHandle, this, &UAttackComponent::AttackLineTrace, AttackInterval, true, 0.f);
	}
}


void UAttackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
//void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

