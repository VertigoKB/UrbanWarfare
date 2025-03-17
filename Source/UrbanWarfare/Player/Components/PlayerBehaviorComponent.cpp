// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	DOREPLIFETIME(UPlayerBehaviorComponent, bCrouching)
	DOREPLIFETIME(UPlayerBehaviorComponent, bWalking)
	//DOREPLIFETIME(UPlayerBehaviorComponent, WalkSpeed)
	//DOREPLIFETIME(UPlayerBehaviorComponent, RunSpeed)
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
	RegInputComp->OnInputWalk.BindUObject(this, &UPlayerBehaviorComponent::ExecuteWalk);

	return true;
}

void UPlayerBehaviorComponent::OnRep_Crouch()
{
	if (bCrouching)
		ThePlayer->Crouch();
	else
		ThePlayer->UnCrouch();
}

void UPlayerBehaviorComponent::ExecuteCrouch(bool bCrouch)
{
	bool IsAuth = ThePlayer->HasAuthority();

	if (IsAuth)
	{
		bCrouching = bCrouch;
		if (ThePlayer->IsLocallyControlled())
			OnRep_Crouch();
	}
	else
		ServerCrouch(bCrouch);
}

void UPlayerBehaviorComponent::ServerCrouch_Implementation(bool bCrouch)
{
	ExecuteCrouch(bCrouch);
}

void UPlayerBehaviorComponent::OnRep_Walk()
{
	float CurrentSpeed = bWalking ? WalkSpeed : RunSpeed;
	ThePlayer->TheMovement->MaxWalkSpeed = CurrentSpeed;
}

void UPlayerBehaviorComponent::ExecuteWalk(bool bWalk)
{
	bool IsAuth = ThePlayer->HasAuthority();

	if (IsAuth)
	{
		bWalking = bWalk;
		if (ThePlayer->IsLocallyControlled())
			OnRep_Walk();
	}
	else
		ServerWalk(bWalk);
}

void UPlayerBehaviorComponent::ServerWalk_Implementation(bool bWalk)
{
	ExecuteWalk(bWalk);
}