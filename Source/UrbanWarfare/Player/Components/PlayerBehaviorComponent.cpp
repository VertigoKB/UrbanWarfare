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
		LOG_EFUNC(TEXT("Failed to initialize component"))
		SetComponentTickEnabled(false);
		return;
	}

	MovementState.Reserve(5);
	MovementState.AddState(EMovementState::Running);
	
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

	DOREPLIFETIME(UPlayerBehaviorComponent, MovementState)
}

bool UPlayerBehaviorComponent::InitConstruct()
{
	ThePlayer = Cast<APlayerBase>(GetOwner());
	if (!ThePlayer)
		return false;

	RegInputComp = Cast<URegisterInputComponent>(ThePlayer->GetRegInputComp());
	if (!RegInputComp)
		return false;

	PlayerMovement = ThePlayer->GetCharacterMovement();
	if (!PlayerMovement)
		return false;

	RegInputComp->OnInputCrouch.BindUObject(this, &UPlayerBehaviorComponent::TriggerCrouch);
	RegInputComp->OnInputWalk.BindUObject(this, &UPlayerBehaviorComponent::ExecuteWalk);
	RegInputComp->OnInputJump.BindUObject(this, &UPlayerBehaviorComponent::ExecuteJump);

	return true;
}

//void UPlayerBehaviorComponent::ClientApplyMovementState_Implementation()
//{
//	if (MovementState.Last() != EMovementState::Crouching)
//		GetOwner<ACharacter>()->UnCrouch();
//
//	switch (MovementState.Last())
//	{
//	case EMovementState::Running:
//		GetOwner<ACharacter>()->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
//		break;
//	case EMovementState::Walking:
//		GetOwner<ACharacter>()->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
//		break;
//	case EMovementState::Crouching:
//		GetOwner<ACharacter>()->Crouch();
//		break;
//	}
//
//	LOG_NETSIMPLE(TEXT("Apply Movement"))
//}

void UPlayerBehaviorComponent::OnRep_MovementState()
{
	if (MovementState.LastState() != EMovementState::Crouching)
		GetOwner<ACharacter>()->UnCrouch();

	switch (MovementState.LastState())
	{
	case EMovementState::Running:
		GetOwner<ACharacter>()->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	case EMovementState::Walking:
		GetOwner<ACharacter>()->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	case EMovementState::Crouching:
		GetOwner<ACharacter>()->Crouch();
		break;
	}
}

void UPlayerBehaviorComponent::TriggerCrouch(bool bCrouch)
{
	if (GetOwner()->HasAuthority())
	{
		if (bCrouch)
		{
			MovementState.AddState(EMovementState::Crouching);
		}
		else
		{
			MovementState.RemoveState(EMovementState::Crouching);
		}
		if (ThePlayer->IsLocallyControlled())
			OnRep_MovementState();
	}
	else
	{
		ServerCrouch(bCrouch);
	}
}

void UPlayerBehaviorComponent::ServerCrouch_Implementation(bool bCrouch)
{
	TriggerCrouch(bCrouch);
}

void UPlayerBehaviorComponent::ExecuteWalk(bool bWalk)
{
	//bool IsAuth = ThePlayer->HasAuthority();
	//
	//if (IsAuth)
	//{
	//	if (bWalk)
	//		MovementState.AddState(EMovementState::Walking);
	//	else
	//		MovementState.RemoveState(EMovementState::Walking);
	//
	//	//ServerSetMovementByState();
	//}
	//else
	//	ServerWalk(bWalk);
}

void UPlayerBehaviorComponent::ServerWalk_Implementation(bool bWalk)
{
	//ExecuteWalk(bWalk);
	////ServerSetMovementByState();
}

void UPlayerBehaviorComponent::ExecuteJump(bool Jump)
{
	ThePlayer->Jump();
}