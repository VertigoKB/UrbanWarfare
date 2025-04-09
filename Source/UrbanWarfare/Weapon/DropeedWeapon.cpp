// Fill out your copyright notice in the Description page of Project Settings.


#include "DropeedWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Common/WarfareLogger.h"
#include "WeaponData/WeaponDataAsset.h"

// Sets default values
ADroppedWeapon::ADroppedWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	SetRootComponent(WeaponRoot);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(WeaponRoot);
	WeaponMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	WeaponMesh->SetSimulatePhysics(false);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(WeaponMesh);
	SphereCollision->InitSphereRadius(70.f);

	SetupComponentsDroppedCollision();
}

// Called when the game starts or when spawned
void ADroppedWeapon::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADroppedWeapon::OnWeaponBeginOverlap);

	bIsWeaponIdSpecified = InitializePlacedWeapon();
}

// Called every frame
//void ADroppedWeapon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ADroppedWeapon::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsWeaponIdSpecified)
		return;

	if (OtherActor->ActorHasTag(FName("Player")))
	{
		if (HasAuthority())
		{
			APlayerBase* OverlappedPlayer = Cast<APlayerBase>(OtherActor);

			 //같은 종류의 무기를 들고있지않아야 함
			if ((OverlappedPlayer->GetWeaponComponent()->IsPlayerHaveThisWeaponType(ThisWeaponType)) == false)
			{
				//https://chatgpt.com/g/g-f52QYAJK1-unreal-engine-5-expert/c/67ef85ee-24d8-8010-984b-cf9a670af542
				OverlappedPlayer->GetWeaponComponent()->LootWeapon(ThisWeaponIdNumber, ThisWeaponType);
				Destroy();
			}
		}
	}
}

bool ADroppedWeapon::ExternalInitialize(const uint8 InIdNumber)
{
	if (bIsWeaponIdSpecified)
	{
		LOG_EFUNC(TEXT("객체를 외부에서 초기화 하고자 하였으나 이미 객체가 초기화되어 있음."));
		return false;
	}

	UWeaponDataAsset* TempWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(InIdNumber);
	if (!TempWeaponData)
	{
		LOG_EFUNC(TEXT("게임 인스턴스에서 WeaponData를 가져오는데 실패하였음."));
		return false;
	}
	WeaponMesh->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
	ThisWeaponType = TempWeaponData->WeaponType;
	ThisWeaponIdNumber = TempWeaponData->WeaponIdNumber;

	SetupComponentsDroppedCollision();
	WeaponMesh->SetSimulatePhysics(true);

	bIsWeaponIdSpecified = true;
	return bIsWeaponIdSpecified;
}

bool ADroppedWeapon::InitializePlacedWeapon()
{
	if (PlacedWeaponInitIdNumber == 0)
		return false;

	UWeaponDataAsset* TempWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(PlacedWeaponInitIdNumber);
	if (!TempWeaponData)
	{
		LOG_EFUNC(TEXT("게임 인스턴스에서 WeaponData를 가져오는데 실패하였음."));
		return false;
	}

	WeaponMesh->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
	ThisWeaponType = TempWeaponData->WeaponType;
	ThisWeaponIdNumber = TempWeaponData->WeaponIdNumber;

	SetupComponentsDroppedCollision();
	WeaponMesh->SetSimulatePhysics(true);
	
	return true;
}

void ADroppedWeapon::SetupComponentsDroppedCollision()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
