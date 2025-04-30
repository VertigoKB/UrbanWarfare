// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/WarfareAnim.h"
#include "UrbanWarfare/Player/Components/RegisterInputComponent.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Player/Components/OptionalClasses/MuzzleFlashSpawner.h"
#include "UrbanWarfare/Player/Components/OptionalClasses/FireTraceHandler.h"
#include "UrbanWarfare/Player/Components/OptionalClasses/AmmoHandler.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Frameworks/GameInstance/ImpactEffectLoader.h"
#include "UrbanWarfare/AssetConfig/ImpactEffectData.h"
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
	DOREPLIFETIME(UCombatComponent, bIsAmmoInMag);
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UCombatComponent::Server_SetbIsAmmoInMag_Implementation(bool InIsAmmoInMag)
{
	bIsAmmoInMag = InIsAmmoInMag;
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//LOG_SIMPLE(TEXT("Host: %d"), bOwnerIsListenHost)
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

	//OwnerPlayerController = GetWorld()->GetFirstPlayerController();
	//if (!OwnerPlayerController)
	//{
	//	LOG_EFUNC(TEXT("Initialization failed: OwnerPlayerController"));
	//	return false;
	//}

	UAnimInstance* FirstAnimInst = OwnerPawn->GetTheMesh()->GetAnimInstance();
	FirstWarfareAnim = Cast<UWarfareAnim>(FirstAnimInst);
	if (!FirstWarfareAnim)
	{
		LOG_EFUNC(TEXT("Initialization failed: FirstAnimInst"));
		return false;
	}

	UAnimInstance* ThirdAnimInst = OwnerPawn->GetThirdMesh()->GetAnimInstance();
	ThirdWarfareAnim = Cast<UWarfareAnim>(ThirdAnimInst);
	if (!ThirdWarfareAnim)
	{
		LOG_EFUNC(TEXT("Initialization failed: WarfareAnim"));
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

	AmmoHandler = WeaponComponent->GetAmmoHandler();
	if (!AmmoHandler)
	{
		LOG_EFUNC(TEXT("Initialization failed: AmmoHandler"));
		return false;
	}

	WeaponPreLoader = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>();
	if (!WeaponPreLoader)
	{
		LOG_EFUNC(TEXT("Initialization failed: WeaponPreLoader"));
		return false;
	}

	ImpactEffectLoader = GetWorld()->GetGameInstance()->GetSubsystem<UImpactEffectLoader>();
	if (!ImpactEffectLoader)
	{
		LOG_EFUNC(TEXT("Initialization failed: ImpactEffectLoader"));
		return false;
	}

	LOG_EFUNC(TEXT("Initialization success."));
	return true;
}

void UCombatComponent::OnSuccessfullyInitialize()
{
	bIsOwnerPawnControlledByHost = OwnerPawn->IsListenHostControlled();

	RegisterInputComponent->OnInputAttack.BindUObject(this, &UCombatComponent::Client_OnStartedInput);
	RegisterInputComponent->OnCompleteAttack.BindUObject(this, &UCombatComponent::Client_OnCompleteInput);
	WeaponComponent->OnWeaponChange.AddUObject(this, &UCombatComponent::OnWeaponChange);

	MuzzleFlashSpawner = NewObject<UMuzzleFlashSpawner>();
	MuzzleFlashSpawner->ExternalInitialize(GetOwner<APlayerBase>());
	FireTraceHandler = NewObject<UFireTraceHandler>();
	FireTraceHandler->ExternalInitialize(GetOwner<APlayerBase>(), this);
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
		Client_PerformAttack();
		IsServer_ProcessContinuousAttack();
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
		IsServer_ProcessContinuousAttack();
	}
}

void UCombatComponent::IsServer_ProcessContinuousAttack()
{
	if (bAuthority)
		GetWorld()->GetTimerManager().SetTimer(RoundIntervalHandle, this, &UCombatComponent::Server_PerformAttack, RoundInterval, true);
	else
		GetWorld()->GetTimerManager().SetTimer(RoundIntervalHandle, this, &UCombatComponent::Client_PerformAttack, RoundInterval, true);
}

void UCombatComponent::Server_ExecuteAttack_Implementation()
{
	Server_PerformAttack();
	bAttackFlag = true;
	//OnRep_bAttackFlag(); // For Host
	IsServer_ProcessContinuousAttack();
}

void UCombatComponent::Server_PerformAttack()
{
	if (bIsReloading || !bIsAmmoInMag)
	{
		bAttackFlag = false;
		GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);
		return;
	}

	MuzzleFlashSpawner->PlayMuzzleEffect();
	FireTraceHandler->AttackLineTrace();
	OnAttack.Broadcast();

	//EWeaponType WeaponType = WeaponComponent->GetEquippedWeaponType();

	//switch (WeaponType)
	//{
	//case EWeaponType::Pistol:
	//	if (bOwnerIsLocal)
	//		FirstWarfareAnim->PlayMontage_FirePistol();
	//	else
	//		ThirdWarfareAnim->PlayMontage_FirePistol();
	//	break;
	//case EWeaponType::Rifle:
	//	if (bOwnerIsLocal)
	//		FirstWarfareAnim->PlayMontage_FireRifle();
	//	else
	//		ThirdWarfareAnim->PlayMontage_FireRifle();
	//	break;
	//}

	if (bOwnerIsListenHost)
		AmmoHandler->Client_Shoot();
}

void UCombatComponent::Client_PerformAttack()
{
	if (bIsReloading || !bIsAmmoInMag)
	{
		bAttackFlag = false;
		GetWorld()->GetTimerManager().ClearTimer(RoundIntervalHandle);
		return;
	}

	MuzzleFlashSpawner->PlayMuzzleEffect();
	OnAttack.Broadcast();

	AmmoHandler->Client_Shoot();

	//EWeaponType WeaponType = WeaponComponent->GetEquippedWeaponType();

	//switch (WeaponType)
	//{
	//case EWeaponType::Pistol:
	//	if (bOwnerIsLocal)
	//		FirstWarfareAnim->PlayMontage_FirePistol();
	//	else
	//		ThirdWarfareAnim->PlayMontage_FirePistol();
	//	break;
	//case EWeaponType::Rifle:
	//	if (bOwnerIsLocal)
	//		FirstWarfareAnim->PlayMontage_FireRifle();
	//	else
	//		ThirdWarfareAnim->PlayMontage_FireRifle();
	//	break;
	//}
}

void UCombatComponent::Multicast_PlayBulletImpact_Implementation(EBulletImpactType InType, FVector_NetQuantize ImpactLocation, FVector_NetQuantizeNormal ImpactNormal)
{
	UImpactEffectData* EffectData = GetWorld()->GetGameInstance()->GetSubsystem<UImpactEffectLoader>()->GetImpactEffectData();
	FTransform ParticleTransform;
	ParticleTransform.SetLocation(FVector(ImpactLocation));
	FQuat ImpactQuat = ImpactNormal.ToOrientationQuat();
	ParticleTransform.SetRotation(ImpactQuat);
	
	switch (InType)
	{
	case EBulletImpactType::Player:
	{
		UParticleSystem* TargetParticle = EffectData->ParticleImpactBody;
		USoundBase* TargetSound = EffectData->SoundImapctBody.Get();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TargetParticle, ParticleTransform,
			true, EPSCPoolMethod::AutoRelease, true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TargetSound, ImpactLocation);
	}
		break;
	case EBulletImpactType::Other:
	{
		UParticleSystem* TargetParticle = EffectData->ParticleImpactEnv;
		USoundBase* TargetSound = EffectData->SoundImpactEnv.Get();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TargetParticle, ParticleTransform,
			true, EPSCPoolMethod::AutoRelease, true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TargetSound, ImpactLocation);
	}
		break;
	}
}
