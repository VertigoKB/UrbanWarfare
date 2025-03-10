// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBehavior.h"
#include "Components/RegisterInputComponent.h"
#include "../Common/WarfareLogger.h"

UPlayerBehavior::UPlayerBehavior()
{
}

bool UPlayerBehavior::InitConstruct(UWorld* World, APlayerBase* Owner)
{
	TheWorld = World;
	if (!TheWorld)
		return false;

	ThePlayer = Owner;
	if (!ThePlayer)
		return false;

	RegInputComp = Cast<URegisterInputComponent>(ThePlayer->GetRegInputComp());
	if (!RegInputComp)
		return false;
	RegInputComp->OnInputCrouch.BindUObject(this, &UPlayerBehavior::ExecuteCrouch);

	bInitFlag = true;
	return bInitFlag;
}

void UPlayerBehavior::ExecuteCrouch(bool bCrouch)
{
	if (!bInitFlag)
		return;

	bool IsAuth = ThePlayer->GetHasAuthority();

	if (IsAuth)
	{

		LOG_NETSIMPLE_UOBJECT(TheWorld, TEXT("__Is Auth"))
		
		if (bCrouch)
		{
			ThePlayer->Crouch();
			LOG_NETSIMPLE_UOBJECT(TheWorld, TEXT("___Do Crouch"))
		}
		else
		{
			LOG_NETSIMPLE_UOBJECT(TheWorld, TEXT("___Do UnCrouch"))
			ThePlayer->UnCrouch();
		}

		bCrouching = bCrouch;
	}
	else
	{	
		LOG_NETSIMPLE_UOBJECT(TheWorld, TEXT("__Not Auth"))
		ThePlayer->ServerCrouch(bCrouch);
	}
}
