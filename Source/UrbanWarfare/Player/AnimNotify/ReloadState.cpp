// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadState.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/CombatComponent.h"
#include "UrbanWarfare/Player/Components/WeaponComponent.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

void UReloadState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerBase* PlayerPawn = MeshComp->GetOwner<APlayerBase>())
	{
		if (PlayerPawn->HasAuthority())
			PlayerPawn->GetCombatComponent()->SetbIsReloading(true);
	}
	else
	{
		LOG_EFUNC(TEXT("Cast failed: PlayerPawn."));
	}
}

void UReloadState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (APlayerBase* PlayerPawn = MeshComp->GetOwner<APlayerBase>())
	{
		if (PlayerPawn->HasAuthority())
		{
			PlayerPawn->GetCombatComponent()->SetbIsReloading(false);

			if (PlayerPawn->IsControlledByLocal())
				PlayerPawn->GetWeaponComponent()->Client_OnCompleteReload();
		}
		else
			PlayerPawn->GetWeaponComponent()->Client_OnCompleteReload();
		
	}
	else
	{
		LOG_EFUNC(TEXT("Cast failed: PlayerPawn."));
	}
}
