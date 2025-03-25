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
	PrimaryComponentTick.TickInterval = 0.1f;
}

// Called when the game starts
void UPlayerSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([this]() {

		bInitFlag = InitConstruct();

		if (bInitFlag)
		{
			SetComponentTickEnabled(true);
			GetWorld()->GetTimerManager().ClearTimer(InitTimer);
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

void UPlayerSoundComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(FootStepHandle);
	GetWorld()->GetTimerManager().ClearTimer(InitTimer);

	LoadedFootSteps = nullptr;
}

// Called every frame
void UPlayerSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Velocity = ThePlayer->GetVelocity();
	float SpeedSquared = Velocity.SizeSquared2D();

	if (SpeedSquared >= MinVelocitySquared - KINDA_SMALL_NUMBER)
		PlayFootStepSound(SpeedSquared);
	else
	{
		if (bPlayingFootSteps)
			StopFootStepSound();
	}
}

void UPlayerSoundComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//SetIsReplicatedByDefault(true);
}

//void UPlayerSoundComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	//DOREPLIFETIME(UPlayerSoundComponent, FootStepLocation);
//}

bool UPlayerSoundComponent::InitConstruct()
{
	ThePlayer = Cast<APlayerBase>(GetOwner());
	if (!ThePlayer)
		return false;

	bAuthority = ThePlayer->HasAuthority();

	RegInputComp = Cast<URegisterInputComponent>(ThePlayer->GetRegInputComp());
	if (!RegInputComp)
		return false;

	PlayerBehavior = Cast<UPlayerBehaviorComponent>(ThePlayer->GetPlayerBehavior());
	if (!PlayerBehavior)
		return false;

	FootStepRef = ThePlayer->BlueprintConfig->FootStepsSound;
	if (!FootStepRef)
		return false;
	
	if (!LoadedFootSteps)
		LoadedFootSteps = FootStepRef.LoadSynchronous();

	return true;
}

// 움직임은 자동으로 리플리케이션되기 때문에, Velocity만 확인하면, 서버로직 없이 발소리 재생 가능
// 키입력에 따른 Server RPC-> Multicast RPC 방식에서 위 방식으로 전환
void UPlayerSoundComponent::PlayFootStepSound(const float SpeedSquared)
{
	if (SpeedSquared < MinVelocitySquared || ThePlayer->IsPlayerFalling())
	{
		StopFootStepSound();
		return;
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

	if (!bPlayingFootSteps)
	{
		bPlayingFootSteps = true;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LoadedFootSteps, ThePlayer->GetActorLocation());
	}

	if (!(GetWorld()->GetTimerManager().IsTimerActive(FootStepHandle)))
	{
		GetWorld()->GetTimerManager().SetTimer(FootStepHandle, FTimerDelegate::CreateLambda([this]() {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), LoadedFootSteps, ThePlayer->GetActorLocation());
			}), FootStepSoundInterval, true);
	}
}

void UPlayerSoundComponent::StopFootStepSound()
{
	bPlayingFootSteps = false;
	GetWorld()->GetTimerManager().ClearTimer(FootStepHandle);
}

/*
void UPlayerSoundComponent::PlayFootStepSound()
{
	FVector Velocity = ThePlayer->GetVelocity();
	float SpeedSquared = Velocity.SizeSquared2D();
	constexpr float MinVelocitySquared = 40.f * 40.f;
	if (SpeedSquared < MinVelocitySquared || ThePlayer->IsPlayerFalling())
	{
		StopFootStepSound();
		return;
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

	if (!bPlayingFootSteps)
	{
		bPlayingFootSteps = true;
		if (ThePlayer->HasAuthority())
			MulticastPlayFootSteps();
		else
			ServerRequestFootStepsSound();
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

*/