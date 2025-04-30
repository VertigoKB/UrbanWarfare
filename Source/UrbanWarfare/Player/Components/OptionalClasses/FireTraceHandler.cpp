// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTraceHandler.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/Components/CombatComponent.h"

void UFireTraceHandler::ExternalInitialize(APlayerBase* const InRootOwner, UCombatComponent* const InComp)
{
	OwnerPawn = InRootOwner;
	bAuthority = OwnerPawn->HasAuthority();
	World = OwnerPawn->GetWorld();
	CombatComponent = InComp;
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

	bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams);
	DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 1.f);

	if (bHit)
	{
		FVector_NetQuantizeNormal BulletDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
		if (HitResult.GetActor()->ActorHasTag(FName("Player")))
			CombatComponent->Multicast_PlayBulletImpact(EBulletImpactType::Player, HitResult.ImpactPoint, BulletDirection);
		else
			CombatComponent->Multicast_PlayBulletImpact(EBulletImpactType::Other, HitResult.ImpactPoint, BulletDirection);
	}

	return bHit;
}
