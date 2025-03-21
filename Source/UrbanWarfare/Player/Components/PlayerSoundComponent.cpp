// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSoundComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
#include "RegisterInputComponent.h"
#include "PlayerBehaviorComponent.h"
#include "../PlayerBase.h"
#include "../Common/WarfareLogger.h"

// Sets default values for this component's properties
UPlayerSoundComponent::UPlayerSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (!InitConstruct())
		return;
	
	//LOG_SIMPLE(TEXT("SoundComponent initialize complete."))
}


// Called every frame
void UPlayerSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerSoundComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetIsReplicatedByDefault(true);
}

bool UPlayerSoundComponent::InitConstruct()
{
	ThePlayer = Cast<APlayerBase>(GetOwner());
	if (!ThePlayer)
	{
		LOG_NULL(ThePlayer)
		return false;
	}

	RegInputComp = Cast<URegisterInputComponent>(ThePlayer->GetRegInputComp());
	if (!RegInputComp)
	{
		LOG_NULL(RegInputComp)
		return false;
	}
	RegInputComp->OnFootStepPlay.BindUObject(this, &UPlayerSoundComponent::PlayFootStepSound);
	RegInputComp->OnStopPlayingFootStep.BindUObject(this, &UPlayerSoundComponent::StopFootStepSound);

	PlayerBehavior = Cast<UPlayerBehaviorComponent>(ThePlayer->GetPlayerBehavior());
	if (!PlayerBehavior)
	{
		LOG_NULL(PlayerBehavior)
		return false;
	}


	FootStepsSound = ThePlayer->BlueprintConfig->FootStepsSound;
	if (!FootStepsSound)
	{
		LOG_NULL(FootStepsSound)
		return false;
	}

	return true;
}

void UPlayerSoundComponent::ServerRequestFootStepsSound_Implementation()
{
	MulticastPlayFootSteps();
}

void UPlayerSoundComponent::MulticastPlayFootSteps_Implementation()
{
	USoundBase* Sound = FootStepsSound.LoadSynchronous();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, ThePlayer->GetActorLocation());
}

void UPlayerSoundComponent::PlayFootStepSound()
{
	float ForwardVelocity = ThePlayer->GetVelocity().X;
	float RightVelocity = ThePlayer->GetVelocity().Y;
	float MinVelocityPlaySound = 100.f;

	if (!(ForwardVelocity > MinVelocityPlaySound ||
		ForwardVelocity < -MinVelocityPlaySound ||
		RightVelocity > MinVelocityPlaySound ||
		RightVelocity < -MinVelocityPlaySound))
	{
		StopFootStepSound();
		return;
	}

	if (ThePlayer->IsPlayerFalling())
	{
		StopFootStepSound();
		return;
	}


	if (!bPlayingFootSteps)
	{
		bPlayingFootSteps = true;
		if (ThePlayer->HasAuthority())
			MulticastPlayFootSteps();
		else
			ServerRequestFootStepsSound();
	}

	if (PlayerBehavior->MovementState.LastState() == EMovementState::Running)
	{
		if (!(FMath::IsNearlyEqual(FootStepSoundInterval, RunInterval)))
			GetWorld()->GetTimerManager().ClearTimer(FootStepHandle);
		FootStepSoundInterval = RunInterval;
	}
	else
	{
		if (!(FMath::IsNearlyEqual(FootStepSoundInterval, WalkInterval)))
			GetWorld()->GetTimerManager().ClearTimer(FootStepHandle);
		FootStepSoundInterval = WalkInterval;
	}

	if (!(GetWorld()->GetTimerManager().IsTimerActive(FootStepHandle)))
	{
		GetWorld()->GetTimerManager().SetTimer(FootStepHandle, FTimerDelegate::CreateLambda([this]() {
			if (ThePlayer->HasAuthority())
				MulticastPlayFootSteps();
			else
				ServerRequestFootStepsSound();
			}), FootStepSoundInterval, true);
	}
}

void UPlayerSoundComponent::StopFootStepSound()
{
	if (FootStepHandle.IsValid())
	{
		bPlayingFootSteps = false;
		GetWorld()->GetTimerManager().ClearTimer(FootStepHandle);
	}
}

