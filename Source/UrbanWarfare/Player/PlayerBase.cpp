// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "WarfareAnim.h"
#include "Components/RegisterInputComponent.h"
#include "Components/PlayerBehaviorComponent.h"
#include "Components/PlayerSoundComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "UrbanWarfare/Common/WarfareLogger.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Frameworks/WarfareGameState.h"
#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"


// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;


	if (!MeshConfig)
	{
		ConstructorHelpers::FObjectFinder<UMeshConfig> DefaultMeshConfig(TEXT("/Game/Blueprints/AssetConfigs/MeshConfig.MeshConfig"));
		if (DefaultMeshConfig.Succeeded())
			MeshConfig = DefaultMeshConfig.Object;
		else
			LOG_NULL(MeshConfig);
	}
	
	if (!BlueprintConfig)
	{
		ConstructorHelpers::FObjectFinder<UBlueprintConfig> DefualtBpConfig(TEXT("/Game/Blueprints/AssetConfigs/BlueprintConfig.BlueprintConfig"));
		if (DefualtBpConfig.Succeeded())
			BlueprintConfig = DefualtBpConfig.Object;
		else
			LOG_NULL(BlueprintConfig);
	}


	SetupBasicComponents();
	SetupCustomComponents();

	Tags.Add(FName("Player"));
}

URegisterInputComponent* APlayerBase::GetRegInputComp() const { return RegisterInputComponent; }
UPlayerBehaviorComponent* APlayerBase::GetPlayerBehavior() const { return PlayerBehavior; }
UPlayerSoundComponent* APlayerBase::GetSoundPlayer() const { return PlayerSoundComponent; }
USkeletalMeshComponent* APlayerBase::GetTheMesh() const { return TheMesh; }
USkeletalMeshComponent* APlayerBase::GetThirdMesh() const { return TheThirdMesh; }
USkeletalMeshComponent* APlayerBase::GetWeaponMesh() const { return WeaponMesh; }
UCapsuleComponent* APlayerBase::GetRootCapsule() const { return TheCapsule; }
UWeaponComponent* APlayerBase::GetWeaponComponent() const { return WeaponComponent; }
//UAttackComponent* APlayerBase::GetAttackComponent() const{ return AttackComponent; }
UCombatComponent* APlayerBase::GetCombatComponent() const { return CombatComponent; }
UHealthComponent* APlayerBase::GetHealthComponent() const { return HealthComponent; }
UCameraComponent* APlayerBase::GetPlayerCamera() const { return TheCamera; }
UBlueprintConfig* APlayerBase::GetBlueprintConfig() const { return BlueprintConfig; }
bool APlayerBase::IsPlayerFalling() const { return TheMovement->IsFalling(); }

void APlayerBase::SetListenHostFlagByController(AWarfareController* InController)
{
	if (HasAuthority() && InController->IsLocalController())
		bIsListenHost = true;

	GetCombatComponent()->bOwnerIsListenHost = true;
}

void APlayerBase::SetMultiplayCaseByController(const EMultiplayCase InMultiplayCase)
{
	MultiplayCase = InMultiplayCase;

	if (MultiplayCase == EMultiplayCase::Server_ListenHost ||
		MultiplayCase == EMultiplayCase::Client_LocalPlayer)
		bIsLocal = true;

	if (MultiplayCase == EMultiplayCase::Server_ListenHost)
		HealthComponent->SetLocalHostFlag();

	CombatComponent->SetLocalFlag(bIsLocal);
	OnspecifiedMultiplayCase.Broadcast(MultiplayCase);
}

void APlayerBase::AttachWeaponMeshToRifle()
{
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	WeaponMesh->AttachToComponent(TheThirdMesh, Rules, FName("RifleSocket"));
}

void APlayerBase::AttachWeaponMeshToPistol()
{
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	WeaponMesh->AttachToComponent(TheThirdMesh, Rules, FName("PistolSocket"));
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	SetupMesh();

	//UWeaponDataAsset* WeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(1);
	//TheRifleMesh->SetSkeletalMesh(WeaponData->WeaponMesh.Get());

	if (IsLocallyControlled())
	{
		FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

		WeaponMesh->AttachToComponent(TheThirdMesh, Rules, FName("RifleSocket"));
	}

	if (this->ActorHasTag(FName("Terrorist")))
		MyTeam = ETeam::Terrorist;
}

void APlayerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 서버에서만 호출됨

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Server
float APlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	bool bIsDead = false;

	HealthComponent->Server_OnDamage(Damage, bIsDead);

	if (bIsDead)
	{
		AWarfareController* WarfareConroller = Cast<AWarfareController>(EventInstigator);
		ETeam EnemyTeam = WarfareConroller->GetMyTeam();

		AWarfareGameState* WorldGameState = GetWorld()->GetGameState<AWarfareGameState>();

		switch (EnemyTeam)
		{
		case ETeam::CounterTrist:
			WorldGameState->Server_AddCounterTristScore();
			break;
		case ETeam::Terrorist:
			WorldGameState->Server_AddTerroristScore();
			break;
		}
	}

	return Damage;
}

void APlayerBase::SetupBasicComponents()
{
	TheCapsule = GetCapsuleComponent();
	//TheCapsule->SetCapsuleRadius(45.f);
	//TheCapsule->SetCapsuleHalfHeight(80.f);

	TheMovement = GetCharacterMovement();
	//TheMovement->MaxWalkSpeed = 700.f;
	//TheMovement->MaxWalkSpeedCrouched = 200.f;
	TheMovement->GetNavAgentPropertiesRef().bCanCrouch = true;
	TheMovement->bCanWalkOffLedgesWhenCrouching = true;

	TheMesh = GetMesh();
	//if (MeshConfig)
	//	TheMesh->SetSkeletalMesh(MeshConfig->CounterTerrorist);
	//TheMesh->SetRelativeLocation(FVector(0.f, 0.f, -80.25f));
	//TheMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	//TheMesh->SetRelativeScale3D(FVector(0.85f, 0.85f, 0.85f));
	TheMesh->bOnlyOwnerSee = true;
	TheMesh->SetCastHiddenShadow(false);

	TheThirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdViewMesh"));
	TheThirdMesh->SetupAttachment(TheMesh);
	//TheThirdMesh->SetSkeletalMesh(MeshConfig->CounterTerrorist);
	TheThirdMesh->CanCharacterStepUpOn = ECB_No;
	//TheThirdMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//TheThirdMesh->bHiddenInGame = true;
	TheThirdMesh->bOwnerNoSee = true;
	TheThirdMesh->SetCastHiddenShadow(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(TheThirdMesh, FName("RifleSocket"));

	TheSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	TheSpringArm->SetupAttachment(TheMesh);
	//TheSpringArm->TargetArmLength = 0.f;
	TheSpringArm->bUsePawnControlRotation = true;
	TheSpringArm->bEnableCameraRotationLag = true;
	TheSpringArm->CameraRotationLagSpeed = 60.f;

	TheCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TheCamera->SetupAttachment(TheSpringArm);
}

void APlayerBase::SetupCustomComponents()
{
	RegisterInputComponent = CreateDefaultSubobject<URegisterInputComponent>(TEXT("RegisterInputComponent"));
	RegisterInputComponent->OnTestInput.BindUObject(this, &APlayerBase::TestUrban);

	PlayerBehavior = CreateDefaultSubobject<UPlayerBehaviorComponent>(TEXT("PlayerBehaviorComponent"));
	PlayerBehavior->SetIsReplicated(true);

	PlayerSoundComponent = CreateDefaultSubobject<UPlayerSoundComponent>(TEXT("PlayerSoundComponent"));

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetIsReplicated(true);
	
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);
}

void APlayerBase::SetupMesh()
{
	if (TheMesh->DoesSocketExist(TEXT("CameraSocket")))
		TheSpringArm->AttachToComponent(TheMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("CameraSocket"));

	TheMesh->HideBoneByName(TEXT("head"), PBO_None);
}

//void APlayerBase::SetupFirstPersonViewSocket()
//{
//	FTimerHandle MeshHandle;
//
//	GetWorld()->GetTimerManager().SetTimer(MeshHandle, FTimerDelegate::CreateLambda([this]() {
//
//		FAttachmentTransformRules TransformRule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
//		TheRifleMesh->AttachToComponent(TheMesh, TransformRule, FName("RifleSocket"));
//		ThePistolMesh->AttachToComponent(TheMesh, TransformRule, FName("PistolSocket"));
//
//		}), 1.f, false);
//}


