// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "UrbanWarfare/Player/Components/CombatComponent.h"
#include "../PlayerBase.h"
#include "RegisterInputComponent.h"
#include "../../Common/WarfareLogger.h"

// Sets default values for this component's properties
UPlayerBehaviorComponent::UPlayerBehaviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UPlayerBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	//SetComponentTickEnabled(false);

	GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([this]() {

		bInitFlag = InitConstruct();

		if (bInitFlag)
		{
			//SetComponentTickEnabled(true);
			GetWorld()->GetTimerManager().ClearTimer(InitTimer);
			MovementState.Reserve(5);
			MovementState.AddState(EMovementState::Running);

			if (ThePlayer->IsControlledByLocal())
				CombatComponent->OnAttack.AddUObject(this, &UPlayerBehaviorComponent::TriggerLook);
		}
		else
		{
			LOG_EFUNC(TEXT("Failed to initialize. Count: %d"), InitCount);
			InitCount++;

			if (InitCount > 10)
			{
				LOG_EFUNC(TEXT("Unable to initialize. Process end."));
				SetActive(false);
				GetWorld()->GetTimerManager().ClearTimer(InitTimer);
			}
		}

		}), 0.5f, true);

}

void UPlayerBehaviorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(SyncAimTimer);
	GetWorld()->GetTimerManager().ClearTimer(FallingChecker);
	GetWorld()->GetTimerManager().ClearTimer(InitTimer);
}

// Called every frame
//void UPlayerBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UPlayerBehaviorComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetIsReplicatedByDefault(true);
}

void UPlayerBehaviorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerBehaviorComponent, MovementState)
	DOREPLIFETIME(UPlayerBehaviorComponent, CurrentSpeed)
	DOREPLIFETIME(UPlayerBehaviorComponent, AimDirection)
}
bool UPlayerBehaviorComponent::InitConstruct()
{
	ThePlayer = Cast<APlayerBase>(GetOwner());
	if (!ThePlayer)
		return false;

	RegInputComp = ThePlayer->GetRegInputComp();
	if (!RegInputComp)
		return false;

	PlayerMovement = ThePlayer->GetCharacterMovement();
	if (!PlayerMovement)
		return false;

	CombatComponent = ThePlayer->GetCombatComponent();
	if (!CombatComponent)
		return false;

	RegInputComp->OnInputCrouch.BindUObject(this, &UPlayerBehaviorComponent::TriggerCrouch);
	RegInputComp->OnInputWalk.BindUObject(this, &UPlayerBehaviorComponent::TriggerWalk);
	RegInputComp->OnInputJump.BindUObject(this, &UPlayerBehaviorComponent::TriggerJump);
	RegInputComp->OnInputLook.BindUObject(this, &UPlayerBehaviorComponent::TriggerLook);

	return true;
}

void UPlayerBehaviorComponent::OnRep_MovementState()
{
	if (!bInitFlag)
		return;

	if (MovementState.LastState() != EMovementState::Crouching)
		GetOwner<ACharacter>()->UnCrouch();

	switch (MovementState.LastState())
	{
	case EMovementState::Running:
		CurrentSpeed = RunSpeed;
		break;
	case EMovementState::Walking:
		CurrentSpeed = WalkSpeed;
		break;
	case EMovementState::Crouching:
		GetOwner<ACharacter>()->Crouch();
		break;
	}

	if (GetOwner()->HasAuthority())
		GetOwner<ACharacter>()->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	else
	{
		GetOwner<ACharacter>()->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
		if (ThePlayer->IsLocallyControlled())
			ServerApplySpeed();
	}
}

void UPlayerBehaviorComponent::ServerApplySpeed_Implementation()
{
	OnRep_MovementState();
}

void UPlayerBehaviorComponent::TriggerCrouch(bool bCrouch)
{
	if (ThePlayer->IsPlayerFalling())
	{
		GetWorld()->GetTimerManager().SetTimer(FallingChecker, FTimerDelegate::CreateLambda([this, bCrouch]() {
			if (!(ThePlayer->IsPlayerFalling()))
			{
				TriggerCrouch(bCrouch);
				GetWorld()->GetTimerManager().ClearTimer(FallingChecker);
			}
			}), 0.05, true);
	}
	else
	{
		if (GetOwner()->HasAuthority())
		{
			if (bCrouch)
				MovementState.AddState(EMovementState::Crouching);
			else
			{
				if (MovementState.ContainState(EMovementState::Crouching))
					MovementState.RemoveState(EMovementState::Crouching);
			}

			if (ThePlayer->IsLocallyControlled())
				OnRep_MovementState();
		}
		else
			ServerCrouch(bCrouch);
	}
}

void UPlayerBehaviorComponent::ServerCrouch_Implementation(bool bCrouch)
{
	TriggerCrouch(bCrouch);
}

void UPlayerBehaviorComponent::TriggerWalk(bool bWalk)
{
	if (GetOwner()->HasAuthority())
	{
		if (bWalk)
			MovementState.AddState(EMovementState::Walking);
		else
			MovementState.RemoveState(EMovementState::Walking);
		if (ThePlayer->IsLocallyControlled())
			OnRep_MovementState();
	}
	else
		ServerWalk(bWalk);
}

void UPlayerBehaviorComponent::ServerWalk_Implementation(bool bWalk)
{
	TriggerWalk(bWalk);
}

void UPlayerBehaviorComponent::TriggerJump(bool Jump)
{
	ThePlayer->Jump();
}

void UPlayerBehaviorComponent::TriggerLook()
{
	if (GetOwner()->HasAuthority())
		AimDirection = DecompressePitch(CalcAimDirection());
	else
		VerifyNecessitySyncAimDirection();
}

void UPlayerBehaviorComponent::ServerLook_Implementation()
{
	AimDirection = DecompressePitch(CalcAimDirection());
}

void UPlayerBehaviorComponent::VerifyNecessitySyncAimDirection()
{
	uint8 NewPitch = DecompressePitch(CalcAimDirection());

	if (FMath::Abs(int32(NewPitch) - int32(AimDirection)) > 5)
	{
		AimDirection = DecompressePitch(CalcAimDirection());
		if (!GetWorld()->GetTimerManager().IsTimerActive(SyncAimTimer))
			GetWorld()->GetTimerManager().SetTimer(SyncAimTimer, this, &UPlayerBehaviorComponent::ServerLook, 0.1f, false);
	}
}

uint8 UPlayerBehaviorComponent::CalcAimDirection()
{
	FRotator ControlRot = ThePlayer->GetControlRotation();
	FRotator ActorRot = ThePlayer->GetActorRotation();

	FRotator TargetRot = UKismetMathLibrary::NormalizedDeltaRotator(ControlRot, ActorRot);

	uint8 CompressedPitch = FMath::Clamp((FMath::RoundToInt(TargetRot.Pitch + 90) * 255 / 180), 0, 255);
	return CompressedPitch;
}
