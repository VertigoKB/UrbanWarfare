// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTraceHandler.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "UrbanWarfare/Player/PlayerBase.h"

void UFireTraceHandler::ExternalInitialize(APlayerBase* InRootOwner)
{
	OwnerPawn = InRootOwner;
	bAuthority = OwnerPawn->HasAuthority();
	World = OwnerPawn->GetWorld();
}

void UFireTraceHandler::BeginDestroy()
{
	Super::BeginDestroy();
	if (World)
		World->GetTimerManager().ClearAllTimersForObject(this);
}

bool UFireTraceHandler::AttackLineTrace()
{
	FHitResult HitResult;
	FVector StartLocation = OwnerPawn->GetPlayerCamera()->GetComponentLocation();
	FVector TargetLocation = OwnerPawn->GetPlayerCamera()->GetForwardVector() * 20000.f;
	FVector EndLocation = StartLocation + TargetLocation;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerPawn);

	bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, CollisionParams);
	DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 1.f);

	return bHit;
}