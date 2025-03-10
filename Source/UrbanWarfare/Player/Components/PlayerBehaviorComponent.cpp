// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBehaviorComponent.h"
#include "../PlayerBase.h"
#include "RegisterInputComponent.h"
#include "../../Common/WarfareLogger.h"

// Sets default values for this component's properties
UPlayerBehaviorComponent::UPlayerBehaviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UPlayerBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (!InitConstruct())
	{
		LOG_EFUNC(TEXT("Failed to Initialize component"))
		SetComponentTickEnabled(false);
		return;
	}

	
}


// Called every frame
void UPlayerBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerBehaviorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UPlayerBehaviorComponent::InitConstruct()
{
	ThePlayer = Cast<APlayerBase>(GetOwner());
	if (!ThePlayer)
		return false;

	RegInputComp = Cast<URegisterInputComponent>(ThePlayer->GetRegInputComp());
	if (!RegInputComp)
		return false;

	RegInputComp->OnInputCrouch.BindUObject(this, &UPlayerBehaviorComponent::ExecuteCrouch);

	return true;
}

void UPlayerBehaviorComponent::ExecuteCrouch(bool bCrouch)
{
	bool IsAuth = ThePlayer->HasAuthority();

	LOG_NETSIMPLE(TEXT("[%s]_Execute"), *FString(ThePlayer->GetName()))

	if (IsAuth)
	{
		ClientCrouch(bCrouch);
		bCrouching = bCrouch;
	}
	else
	{
		LOG_NETSIMPLE(TEXT("__NoAuth"))
		ServerCrouch(bCrouch);
	}
}

void UPlayerBehaviorComponent::ClientCrouch_Implementation(bool bCrouch)
{
	bCrouch ? ThePlayer->Crouch() : ThePlayer->UnCrouch();
}

void UPlayerBehaviorComponent::ServerCrouch_Implementation(bool bCrouch)
{
	LOG_NETSIMPLE(TEXT("___Try remote"))
	ExecuteCrouch(bCrouch);
}