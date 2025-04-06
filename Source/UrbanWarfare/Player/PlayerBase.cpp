// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "Components/RegisterInputComponent.h"
#include "Components/PlayerBehaviorComponent.h"
#include "Components/PlayerSoundComponent.h"
#include "Components/WeaponComponent.h"
#include "UrbanWarfare/Common/WarfareLogger.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"


// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
USkeletalMeshComponent* APlayerBase::GetRifleMesh() const { return TheRifleMesh; }
UWeaponComponent* APlayerBase::GetWeaponComponent() const { return WeaponComponent; }
UBlueprintConfig* APlayerBase::GetBlueprintConfig() const { return BlueprintConfig; }
bool APlayerBase::IsPlayerFalling() const { return TheMovement->IsFalling(); }

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	SetupMesh();

	UWeaponDataAsset* WeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByName("AK47");
	TheRifleMesh->SetSkeletalMesh(WeaponData->WeaponMesh.Get());

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

	TheRifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));
	TheRifleMesh->SetupAttachment(TheMesh, FName("RifleSocket"));

	ThePistolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PistolMesh"));
	ThePistolMesh->SetupAttachment(TheMesh, FName("PistolSocket"));

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
}

void APlayerBase::SetupMesh()
{
	if (TheMesh->DoesSocketExist(TEXT("CameraSocket")))
		TheSpringArm->AttachToComponent(TheMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("CameraSocket"));

	TheMesh->HideBoneByName(TEXT("head"), PBO_None);

	if (DebugCamera)
		TheSpringArm->TargetArmLength = 600.f;
}


