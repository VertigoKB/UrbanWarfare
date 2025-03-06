// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerInitilize();

	if (!MeshConfig)
	{
		ConstructorHelpers::FObjectFinder<UMeshConfig> DefaultMeshConfig(TEXT("/Game/Blueprints/AssetConfigs/MeshConfig.MeshConfig"));
		if (DefaultMeshConfig.Succeeded())
			MeshConfig = DefaultMeshConfig.Object;
		else
			UE_LOG(LogTemp, Log, TEXT("[APlayerBase::APlayerBase] MeshConfig is null."));
	}

	TheCapsule = GetCapsuleComponent();
	TheCapsule->SetCapsuleRadius(34.f);
	TheCapsule->SetCapsuleHalfHeight(93.f);

	TheMovement = GetCharacterMovement();
	TheMovement->MaxWalkSpeed = 250.f;

	TheMesh = GetMesh();
	if (MeshConfig)
		TheMesh->SetSkeletalMesh(MeshConfig->CounterTerrorist);
	TheMesh->SetRelativeLocation(FVector(0.f, 0.f, -93.5f));
	TheMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	TheSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	TheSpringArm->SetupAttachment(TheMesh);
	TheSpringArm->SetRelativeLocation(FVector());
	TheSpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	TheSpringArm->TargetArmLength = 600.f;

	TheCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TheCamera->SetupAttachment(TheSpringArm);

}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
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

bool APlayerBase::PlayerInitilize()
{
	return false;
}

