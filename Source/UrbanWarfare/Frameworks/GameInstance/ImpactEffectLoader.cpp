// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactEffectLoader.h"
#include "Engine/AssetManager.h"
#include "UrbanWarfare/AssetConfig/ImpactEffectData.h"
#include "UrbanWarfare/Common/WarfareLogger.h"


void UImpactEffectLoader::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnCompleteLoad_A.BindLambda([this]() {
		bCompleteA = true;
		CheckAsyncLoadComplete();
		});
	OnCompleteLoad_B.BindLambda([this]() {
		bCompleteB = true;
		CheckAsyncLoadComplete();
		});
	//OnCompleteLoad_C.BindLambda([this]() {
	//	bCompleteC = true;
	//	CheckAsyncLoadComplete();
	//	});
	//OnCompleteLoad_D.BindLambda([this]() {
	//	bCompleteD = true;
	//	CheckAsyncLoadComplete();
	//	});

	LoadEffectData();
}

void UImpactEffectLoader::Deinitialize()
{
	Super::Deinitialize();
}

bool UImpactEffectLoader::LoadEffectData()
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ImpactEffect"), Assets);

	EffectDataCount = Assets.Num();

	for (const auto& AssetId : Assets)
	{
		Manager.LoadPrimaryAsset(
			AssetId,
			TArray<FName>{},
			FStreamableDelegate::CreateUObject(
				this,
				&UImpactEffectLoader::OnEffectAssetLoaded,
				AssetId
			)
		);
	}

	return true;
}

UImpactEffectData* UImpactEffectLoader::GetImpactEffectData()
{
	return EffectData;
}

void UImpactEffectLoader::OnEffectAssetLoaded(FPrimaryAssetId LoadedAssetId)
{
	UAssetManager& Manager = UAssetManager::Get();
	UObject* LoadedObject = Manager.GetPrimaryAssetObject(LoadedAssetId);

	UImpactEffectData* LoadedData = Cast<UImpactEffectData>(LoadedObject);

	if (!LoadedData)
	{
		LOG_SIMPLE(TEXT("Failed to load weapon asset."));
		return;
	}

	if (LoadedData->SoundImapctBody.IsPending())
	{
		StreamableManager.RequestAsyncLoad(
			LoadedData->SoundImapctBody.ToSoftObjectPath(),
			[this]() {
				LOG_SIMPLE(TEXT("SoundImapctBody loaded Asynchronously"));
				OnCompleteLoad_A.ExecuteIfBound();
			}
		);
	}
	else
	{
		if (LoadedData->SoundImapctBody.IsValid())
		{
			LOG_SIMPLE(TEXT("SoundImapctBody loaded with initialize."));
			bCompleteA = true;
		}
	}

	if (LoadedData->SoundImpactEnv.IsPending())
	{
		StreamableManager.RequestAsyncLoad(
			LoadedData->SoundImpactEnv.ToSoftObjectPath(),
			[this]() {
				LOG_SIMPLE(TEXT("SoundImpactEnv loaded Asynchronously"));
				OnCompleteLoad_B.ExecuteIfBound();
			}
		);
	}
	else
	{
		if (LoadedData->SoundImpactEnv.IsValid())
		{
			LOG_SIMPLE(TEXT("SoundImpactEnv loaded with initialize."));
			bCompleteB = true;
		}
	}

	//if (LoadedData->ParticleImpactBody.IsPending())
	//{
	//	StreamableManager.RequestAsyncLoad(
	//		LoadedData->ParticleImpactBody.ToSoftObjectPath(),
	//		[this]() {
	//			LOG_SIMPLE(TEXT("ParticleImpactBody loaded Asynchronously"));
	//			OnCompleteLoad_C.ExecuteIfBound();
	//		}
	//	);
	//}
	//else
	//{
	//	if (LoadedData->ParticleImpactBody.IsValid())
	//	{
	//		LOG_SIMPLE(TEXT("ParticleImpactBody loaded with initialize."));
	//		bCompleteC = true;
	//	}
	//}

	//if (LoadedData->ParticleImpactEnv.IsPending())
	//{
	//	StreamableManager.RequestAsyncLoad(
	//		LoadedData->ParticleImpactEnv.ToSoftObjectPath(),
	//		[this]() {
	//			LOG_SIMPLE(TEXT("ParticleImpactEnv loaded Asynchronously"));
	//			OnCompleteLoad_D.ExecuteIfBound();
	//		}
	//	);
	//}
	//else
	//{
	//	if (LoadedData->ParticleImpactEnv.IsValid())
	//	{
	//		LOG_SIMPLE(TEXT("ParticleImpactEnv loaded with initialize."));
	//		bCompleteD = true;
	//	}
	//}


	EffectData = LoadedData;

	LOG_SIMPLE(TEXT("Loaded Effect Asset: %s"), *EffectData->GetName());
}

void UImpactEffectLoader::CheckAsyncLoadComplete()
{
	if (bCompleteA && bCompleteB /*&& bCompleteC && bCompleteD*/)
		bIsLoadSuccess = true;
}
