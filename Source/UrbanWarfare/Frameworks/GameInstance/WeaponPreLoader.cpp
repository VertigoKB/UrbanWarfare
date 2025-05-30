// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPreLoader.h"
#include "Engine/AssetManager.h"

#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

void UWeaponPreLoader::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GetWorld())
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this]() {

			WeaponDataMap.Reserve(5);

			LoadWeaponDataAsset();

			}), 0.1f, false);
	}


}

void UWeaponPreLoader::Deinitialize()
{
	Super::Deinitialize();
}

bool UWeaponPreLoader::LoadWeaponDataAsset()
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("Weapon"), Assets);

	for (const auto& AssetId : Assets)

	{
		Manager.LoadPrimaryAsset(
			AssetId,
			TArray<FName>{},
			FStreamableDelegate::CreateUObject(
				this,
				&UWeaponPreLoader::OnWeaponDataLoaded,
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
	//	WeaponData->InitiallizeValues();
	//	WeaponData->WeaponMesh.LoadSynchronous();

	//	WeaponDataMap.Add(WeaponData->WeaponIdNumber, WeaponData);
	//}

	LOG_SIMPLE(TEXT("UWeaponPreLoader Load Success"));
	return true;
}

void UWeaponPreLoader::OnWeaponDataLoaded(FPrimaryAssetId LoadedAssetId)
{
	UAssetManager& Manager = UAssetManager::Get();
	UObject* LoadedObject = Manager.GetPrimaryAssetObject(LoadedAssetId);

	UWeaponDataAsset* LoadedData = Cast<UWeaponDataAsset>(LoadedObject);

	if (LoadedData)
	{
		LoadedData->InitiallizeValues();
		
		if (LoadedData->WeaponMesh.IsPending())
		{
			StreamableManager.RequestAsyncLoad(
				LoadedData->WeaponMesh.ToSoftObjectPath(),
				[]() {}
			);
		}

		WeaponDataMap.Add(LoadedData->WeaponIdNumber, LoadedData);
	}


	
}

UWeaponDataAsset* UWeaponPreLoader::GetWeaponDataByWeaponId(uint8 InNumber)
{
	if (this)
	{
		if (WeaponDataMap.Contains(InNumber))
		{
			UWeaponDataAsset** TargetData = WeaponDataMap.Find(InNumber);
			return *TargetData;
		}
		else
		{
			UWeaponDataAsset** ErrorData = WeaponDataMap.Find(1);
			//UWeaponPreLoader에 잘못된 데이터가 요청되어 ID_1 무기가 반환됨.
			//ensure(false);
			LOG_WARNFUNC(TEXT("존재하지 않는 무기 데이터가 요청되어 Num_1 무기가 반환됨. 요청번호: %d"), InNumber);
			return *ErrorData;
		}
	}
	else
	{
		LOG_WARNFUNC(TEXT("월드가 삭제되어 무기 데이터를 제공할 수 없음. 요청번호: %d"), InNumber);
		return nullptr;
	}
}
