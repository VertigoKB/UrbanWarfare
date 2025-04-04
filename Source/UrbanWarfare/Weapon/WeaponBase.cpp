// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	SetRootComponent(WeaponRoot);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(WeaponRoot);
	WeaponMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	WeaponMesh->SetSimulatePhysics(true);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(WeaponMesh);
	SphereCollision->InitSphereRadius(70.f);

	SetupComponentsCollision();
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	SetupComponentsCollision();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnWeaponBeginOverlap);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		if (HasAuthority())
		{
			APlayerBase* OverlappedPlayer = Cast<APlayerBase>(OtherActor);

			// 같은 종류의 무기를 들고있지않아야 함
			if ((OverlappedPlayer->GetWeaponComponent()->IsPlayerHaveThisWeaponType(WeaponType)) == false)
			{
				//https://chatgpt.com/g/g-f52QYAJK1-unreal-engine-5-expert/c/67ef85ee-24d8-8010-984b-cf9a670af542
				OverlappedPlayer->GetWeaponComponent()->LootWeapon(WeaponItem);
				Destroy();
			}
		}
	}
}

void AWeaponBase::SetupComponentsCollision()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

