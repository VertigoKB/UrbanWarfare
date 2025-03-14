// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/RegisterInputComponent.h"
#include "Components/PlayerBehaviorComponent.h"
#include "../Common/WarfareLogger.h"


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
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

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

void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APlayerBase, RegisterInputComponent);
	DOREPLIFETIME(APlayerBase, PlayerBehavior);
	DOREPLIFETIME(APlayerBase, CurrentSpeed);
}

void APlayerBase::SetupBasicComponents()
{
	TheCapsule = GetCapsuleComponent();
	TheCapsule->SetCapsuleRadius(34.f);
	TheCapsule->SetCapsuleHalfHeight(93.f);

	TheMovement = GetCharacterMovement();
	TheMovement->MaxWalkSpeed = 600.f;
	TheMovement->GetNavAgentPropertiesRef().bCanCrouch = true;
	TheMovement->bCanWalkOffLedgesWhenCrouching = true;

	TheMesh = GetMesh();
	if (MeshConfig)
		TheMesh->SetSkeletalMesh(MeshConfig->CounterTerrorist);
	if (BlueprintConfig)
		TheMesh->AnimClass = BlueprintConfig->PlayerAnimBP;
	TheMesh->SetRelativeLocation(FVector(0.f, 0.f, -93.5f));
	TheMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	TheSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	TheSpringArm->SetupAttachment(TheMesh);
	TheSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 142.f));
	TheSpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	TheSpringArm->TargetArmLength = 600.f;
	TheSpringArm->bUsePawnControlRotation = true;
	TheSpringArm->bEnableCameraLag = true;

	TheCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TheCamera->SetupAttachment(TheSpringArm);
}

void APlayerBase::SetupCustomComponents()
{
	RegisterInputComponent = CreateDefaultSubobject<URegisterInputComponent>(TEXT("RegisterInputComponent"));
	PlayerBehavior = CreateDefaultSubobject<UPlayerBehaviorComponent>(TEXT("PlayerBehaviorComponent"));
}



void APlayerBase::ServerSetWalkSpeed_Implementation(bool bWalk)
{
	CurrentSpeed = bWalk ? WalkSpeed : RunSpeed;
	TheMovement->MaxWalkSpeed = CurrentSpeed;
}

UActorComponent* APlayerBase::GetRegInputComp()
{
	return RegisterInputComponent;
}

UActorComponent* APlayerBase::GetPlayerBehavior()
{
	return PlayerBehavior;
}

void APlayerBase::OnRep_ChangeMaxWalkSpeed()
{
	TheMovement->MaxWalkSpeed = CurrentSpeed;
}
