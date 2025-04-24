// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/RegisterInputComponent.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Player/Components/OptionalClasses/MuzzleFlashSpawner.h"
#include "UrbanWarfare/Player/Components/OptionalClasses/FireTraceHandler.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Common/WarfareLogger.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, bAttackFlag);
	DOREPLIFETIME(UCombatComponent, bIsReloading);
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	bIsInitialized = InitConstruct();
	if (!bIsInitialized)
	{
		GetWorld()->GetTimerManager().SetTimer(InitHandle, FTimerDelegate::CreateLambda([this]() {

			bIsInitialized = InitConstruct();
			if (bIsInitialized)
			{
				OnSuccessfullyInitialize();
				GetWorld()->GetTimerManager().ClearTimer(InitHandle);
			}
			else
			{
				if (InitCount > 0)
					InitCount--;
				else
					GetWorld()->GetTimerManager().ClearTimer(InitHandle);
			}

			}), 0.2f, true);
	}
	else
	{
		LOG_EFUNC(TEXT("Initialization success."));
		OnSuccessfullyInitialize();
	}
	
}

bool UCombatComponent::InitConstruct()
{

	OwnerPawn = GetOwner<APlayerBase>();
	if (!OwnerPawn)
	{
		LOG_EFUNC(TEXT("Initialization failed: OwnerPawn"));
		return false;
	}

	bAuthority = OwnerPawn->HasAuthority();
	if (bAuthority && OwnerPawn->IsLocallyControlled())
		bIsListen = true;

	OwnerPlayerController = GetWorld()->GetFirstPlayerController();
	if (!OwnerPlayerController)
	{
		LOG_EFUNC(TEXT("Initialization failed: OwnerPlayerController"));
		return false;
	}

	RegisterInputComponent = OwnerPawn->GetRegInputComp();
	if (!RegisterInputComponent)
	{
		LOG_EFUNC(TEXT("Initialization failed: RegisterInputComponent"));
		return false;
	}

	WeaponComponent = OwnerPawn->GetWeaponComponent();
	if (!WeaponComponent)
	{
		LOG_EFUNC(TEXT("Initialization failed: WeaponComponent"));
		return false;
	}

	WeaponPreLoader = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>();
	if (!WeaponPreLoader)
	{
		LOG_EFUNC(TEXT("Initialization failed: WeaponPreLoader"));
		return false;
	}

	LOG_EFUNC(TEXT("Initialization success."));
	return true;
}

void UCombatComponent::OnSuccessfullyInitialize()
{
	RegisterInputComponent->OnInputAttack.BindUObject(this, &UCombatComponent::Client_OnStartedInput);
	RegisterInputComponent->OnCompleteAttack.BindUObject(this, &UCombatComponent::Client_OnCompleteInput);
	WeaponComponent->OnWeaponChange.AddUObject(this, &UCombatComponent::OnWeaponChange);

	MuzzleFlashSpawner = NewObject<UMuzzleFlashSpawner>();
	MuzzleFlashSpawner->ExternalInitialize(GetOwner<APlayerBase>());
	FireTraceHandler = NewObject<UFireTraceHandler>();
	FireTraceHandler->ExternalInitialize(GetOwner<APlayerBase>());
}

void UCombatComponent::Client_OnStartedInput()
{
	Server_ExecuteAttack();
}

void UCombatComponent::Client_OnCompleteInput()
{
	GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);
	Server_StopContinuousAttack();
}

void UCombatComponent::Server_StopContinuousAttack_Implementation()
{
	bAttackFlag = false;
	GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);
}

void UCombatComponent::OnRep_bAttackFlag()
{
	if (bAttackFlag)
	{
		ExecuteAttack();
		ProcessContinuousAttack();
	}
	else
		GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);
}

void UCombatComponent::OnWeaponChange(uint8 InWeaponId)
{
	UWeaponDataAsset* EquippedWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(InWeaponId);
	RoundInterval = EquippedWeaponData->RoundInterval;
	Damage = EquippedWeaponData->Damage;

	if (GetWorld()->GetTimerManager().IsTimerActive(RoundIntervalHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);
		ProcessContinuousAttack();
	}
}

void UCombatComponent::ProcessContinuousAttack()
{
	GetWorld()->GetTimerManager().SetTimer(RoundIntervalHandle, this, &UCombatComponent::ExecuteAttack, RoundInterval, true);
}

void UCombatComponent::Server_ExecuteAttack_Implementation()
{
	ExecuteAttack();
	bAttackFlag = true;
	ProcessContinuousAttack();
	OnRep_bAttackFlag(); // For Host
}

void UCombatComponent::ExecuteAttack()
{
	if (bIsReloading)
		return;

	MuzzleFlashSpawner->PlayMuzzleEffect();
	if (bAuthority)
		FireTraceHandler->AttackLineTrace();
	OnAttack.Broadcast();
}
