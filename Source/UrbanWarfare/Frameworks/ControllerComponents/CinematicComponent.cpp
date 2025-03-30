// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "CineCameraActor.h"

// Sets default values for this component's properties
UCinematicComponent::UCinematicComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SceneCameras.Reserve(4);
	SequenceActors.Reserve(4);
	// ...
}


// Called when the game starts
void UCinematicComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACineCameraActor::StaticClass(), SceneCameras);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), SequenceActors);

	for (uint8 i = 0; i < SceneCameras.Num() - 1; i++)
	{
		if (!CineCameraA)
			CineCameraA = Cast<ACineCameraActor>(SceneCameras[i]);
		else if (!CineCameraB)
			CineCameraB = Cast<ACineCameraActor>(SceneCameras[i]);
	}

	SceneCameras.Empty();

	APlayerController* OwnerController = Cast<APlayerController>(GetOwner());
	OwnerController->SetViewTarget(CineCameraA);
	
}


// Called every frame
void UCinematicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

