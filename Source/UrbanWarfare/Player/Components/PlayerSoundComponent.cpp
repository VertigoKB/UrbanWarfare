// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSoundComponent.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/SoundPlayer/FootStepPlayer.h"
#include "UrbanWarfare/Player/SoundPlayer/GunShotPlayer.h"


//#include "Kismet/GameplayStatics.h"
//#include "UrbanWarfare/AssetConfig/BlueprintConfig.h"
//#include "RegisterInputComponent.h"
//#include "PlayerBehaviorComponent.h"
//#include "../PlayerBase.h"
//#include "../Common/WarfareLogger.h"

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

	FootStepPlayer = NewObject<UFootStepPlayer>();
	FootStepPlayer->ExternalInitialize(GetOwner<APlayerBase>());

	GunShotPlayer = NewObject<UGunShotPlayer>();
	GunShotPlayer->ExternalInitialize(GetOwner<APlayerBase>());
}
void UPlayerSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FootStepPlayer->CalculatePlayerVelocity();
}