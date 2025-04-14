// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPreLoader.h"

#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

void UWeaponPreLoader::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	WeaponDataMap.Reserve(5);

	//for (uint8 i = 0; i < 10; i++)
	//{
	//	bIsLoadSuccess = LoadWeaponDataAsset();
	//	if (bIsLoadSuccess)
	//		break;
	//}
	LoadWeaponDataAsset();
	
}

void UWeaponPreLoader::Deinitialize()
{
	Super::Deinitialize();

	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UWeaponPreLoader::LoadWeaponDataAsset()
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("Weapon"), Assets);

	WeaponDataCount = Assets.Num();

	for (const auto& AssetId : Assets)
	{
		Manager.LoadPrimaryAsset(
			AssetId,
			TArray<FName>{},
			FStreamableDelegate::CreateUObject(
				this,
				&UWeaponPreLoader::OnWeaponAssetLoaded,
				AssetId
			)
		);
	}


	//FSoftObjectPtr AssetPtr;

	//for (const auto& Iter : Assets)
	//{
	//	AssetPtr = Manager.GetPrimaryAssetPath(Iter);
	//	if (AssetPtr.IsPending())
	//		AssetPtr.LoadSynchronous();
	//	

	//	UWeaponDataAsset* WeaponData = Cast<UWeaponDataAsset>(AssetPtr.Get());
	//	if (!WeaponData)
	//	{
	//		LOG_SIMPLE(TEXT("UWeaponPreLoader failed to load"));
	//		return false;
	//	}
	//	WeaponData->WeaponMesh.LoadSynchronous();

	//	WeaponDataMap.Add(WeaponData->WeaponIdNumber, WeaponData);
	//}

	//LOG_SIMPLE(TEXT("UWeaponPreLoader Load Success"));
	//return true;
}

UWeaponDataAsset* UWeaponPreLoader::GetWeaponDataByWeaponId(uint8 InNumber)
{
	UWeaponDataAsset** TargetData = WeaponDataMap.Find(InNumber);

	// 게임 인스턴스에 존재하지 않는 무기의 데이터를 요청하면, 여기서 중단됩니다.
	return *TargetData;
}

void UWeaponPreLoader::OnWeaponAssetLoaded(FPrimaryAssetId LoadedAssetId)
{
	UAssetManager& Manager = UAssetManager::Get();
	UObject* LoadedObject = Manager.GetPrimaryAssetObject(LoadedAssetId);
	
	UWeaponDataAsset* WeaponData = Cast<UWeaponDataAsset>(LoadedObject);

	if (!WeaponData)
	{
		LOG_SIMPLE(TEXT("Failed to load weapon asset."));
		return;
	}

	if (WeaponData->WeaponMesh.IsPending())
	{
		StreamableManager.RequestAsyncLoad(
			WeaponData->WeaponMesh.ToSoftObjectPath(),
			[=]() {
				LOG_SIMPLE(TEXT("Weapon mesh loaded Asynchronously"));
			}
		);
	}
	WeaponDataMap.Add(WeaponData->WeaponIdNumber, WeaponData);

	if (WeaponDataMap.Num() == WeaponDataCount)
		bLoadComplete = true;

	LOG_SIMPLE(TEXT("Loaded Weapon Asset: %s"), *WeaponData->GetName());
}
