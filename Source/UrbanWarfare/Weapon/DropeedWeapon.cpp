// Fill out your copyright notice in the Description page of Project Settings.


#include "DropeedWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Frameworks/GameInstance/WeaponPreLoader.h"
#include "UrbanWarfare/Common/CommonStructs.h"
#include "UrbanWarfare/Common/WarfareLogger.h"
#include "WeaponData/WeaponDataAsset.h"

// Sets default values
ADroppedWeapon::ADroppedWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	//WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	//SetRootComponent(WeaponRoot);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	//WeaponMesh->SetupAttachment(WeaponRoot);
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	//WeaponMesh->SetUpdateKinematicFromSimulation(true);
	//WeaponMesh->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::ComponentTransformIsKinematic;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(WeaponMesh);
	SphereCollision->InitSphereRadius(70.f);

	//WeaponLocation = FVector_NetQuantize(FVector(0.f, 0.f, 0.f));

	SetupComponentsDroppedCollision();
}

void ADroppedWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (WeaponMesh && WeaponMesh->GetAttachParent() == WeaponMesh)
	{
		WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		UE_LOG(LogTemp, Warning, TEXT("무기 메쉬가 자기 자신에 붙으려는 시도를 감지하고 무시함"));
	}
}

// Called when the game starts or when spawned
void ADroppedWeapon::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADroppedWeapon::OnWeaponBeginOverlap);

	bIsWeaponIdSpecified = InitializePlacedWeapon();

	//if (HasAuthority())
	//{
	//	WeaponMesh->SetSimulatePhysics(true);
	//	SetActorTickEnabled(false);
	//
	//	GetWorld()->GetTimerManager().SetTimer(SynchronizationHandle, FTimerDelegate::CreateLambda([this]() {
	//
	//		WeaponLocation = GetActorLocation();
	//		WeaponRotation = GetActorRotation().Vector();
	//
	//		}), 0.1f, true);
	//}
	//else
	//{
	//	WeaponMesh->SetSimulatePhysics(false);
	//	SetActorTickEnabled(false);
	//}
}

void ADroppedWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

// 틱이 활성화 되었다는 것은 WeaponLocation이 서버에서 변화했다는 것이다.
void ADroppedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//FVector NewLocation = FMath::VInterpTo(GetActorLocation(), WeaponLocation, DeltaTime, 50000.f);
	//FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), WeaponRotation.Rotation(), DeltaTime, 50000.f);

	//SetActorLocationAndRotation(NewLocation, NewRotation.Quaternion());
}

void ADroppedWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADroppedWeapon, bIsWeaponIdSpecified);
	DOREPLIFETIME(ADroppedWeapon, ThisWeaponIdNumber);
	DOREPLIFETIME(ADroppedWeapon, ThisWeaponType);
	DOREPLIFETIME(ADroppedWeapon, AmmoInMag);
	DOREPLIFETIME(ADroppedWeapon, ExtraAmmo);
}

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
				FDroppedWeaponData Data;
				Data.WeaponId = ThisWeaponIdNumber;
				Data.WeaponType = ThisWeaponType;
				Data.AmmoInMag = AmmoInMag;
				Data.ExtraAmmo = ExtraAmmo;
				//https://chatgpt.com/g/g-f52QYAJK1-unreal-engine-5-expert/c/67ef85ee-24d8-8010-984b-cf9a670af542
				OverlappedPlayer->GetWeaponComponent()->LootWeapon(Data);
				Destroy();
			}
		}
	}
}

bool ADroppedWeapon::ExternalInitialize(const uint8 InIdNumber, FWeaponAmmoData AmmoData)
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
	
	AmmoInMag = AmmoData.AmmoInMag;
	ExtraAmmo = AmmoData.ExtraAmmo;

	SetupComponentsDroppedCollision();
	WeaponMesh->SetSimulatePhysics(true);

	bIsWeaponIdSpecified = true;
	return bIsWeaponIdSpecified;
}

USkeletalMeshComponent* ADroppedWeapon::GetWeaponMesh() const { return WeaponMesh; }

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
	AmmoInMag = TempWeaponData->LoadableAmmoPerMag;
	ExtraAmmo = (TempWeaponData->MaxAmmo) - (TempWeaponData->LoadableAmmoPerMag);

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

void ADroppedWeapon::OnRep_bIsWeaponIdSpecified()
{
	if (!(ThisWeaponIdNumber > 0))
		return;

	UWeaponDataAsset* TempWeaponData = GetWorld()->GetGameInstance()->GetSubsystem<UWeaponPreLoader>()->GetWeaponDataByWeaponId(ThisWeaponIdNumber);
	WeaponMesh->SetSkeletalMesh(TempWeaponData->WeaponMesh.Get());
	WeaponMesh->SetSimulatePhysics(true);
	SetupComponentsDroppedCollision();
}

//void ADroppedWeapon::OnRep_WeaponLocation()
//{
//	SetActorTickEnabled(true);
//}
