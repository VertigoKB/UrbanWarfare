// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBehavior.h"
#include "RegisterInputComponent.h"

UPlayerBehavior::UPlayerBehavior()
{
}

bool UPlayerBehavior::InitConstruct(APlayerBase* Owner)
{
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
		if (bCrouch)
			ThePlayer->Crouch();
		else
			ThePlayer->UnCrouch();

		bCrouching = bCrouch;
	}
	else
		ThePlayer->ServerCrouch(bCrouch);
}
