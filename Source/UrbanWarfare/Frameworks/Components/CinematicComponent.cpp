// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "CineCameraActor.h"
#include "UrbanWarfare/Frameworks/WarfareHud.h"

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
	InitConstruct();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACineCameraActor::StaticClass(), SceneCameras);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), SequenceActors);

	for (const auto& Iter : SceneCameras)
	{
		if (Iter->Tags[0] == FName(TEXT("SceneCameraA")))
			CineCameraA = Cast<ACineCameraActor>(Iter);
		else if (Iter->Tags[0] == FName(TEXT("SceneCameraB")))
			CineCameraB = Cast<ACineCameraActor>(Iter);
	}

	for (const auto& Iter : SequenceActors)
	{
		if (Iter->Tags[0] == FName(TEXT("SceneA")))
			SequenceActorA = Cast<ALevelSequenceActor>(Iter);
		else if (Iter->Tags[0] == FName(TEXT("SceneB")))
			SequenceActorB = Cast<ALevelSequenceActor>(Iter);
	}

	SceneCameras.Empty();
	SequenceActors.Empty();

	

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	ALevelSequenceActor* TempSequenceActorA = SequenceActorA;
	ALevelSequenceActor* TempSequenceActorB = SequenceActorB;
	SequencePlayerA = ULevelSequencePlayer::CreateLevelSequencePlayer(TempSequenceActorA->GetWorld(), TempSequenceActorA->GetSequence(), PlaybackSettings, TempSequenceActorA);
	SequencePlayerB = ULevelSequencePlayer::CreateLevelSequencePlayer(TempSequenceActorB->GetWorld(), TempSequenceActorB->GetSequence(), PlaybackSettings, TempSequenceActorB);

	SequencePlayerA->OnFinished.AddDynamic(this, &UCinematicComponent::PlaySequenceB);
	SequencePlayerB->OnFinished.AddDynamic(this, &UCinematicComponent::PlaySequenceA);
	
	PlaySequenceA();
}


// Called every frame
void UCinematicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCinematicComponent::InitConstruct()
{
	AWarfareHud* Hud = GetOwner<AWarfareHud>();
	if (Hud)
		OwnerController = Hud->GetOwningPlayerController();

	return true;
}

void UCinematicComponent::PlaySequenceA()
{
	OwnerController->SetViewTarget(CineCameraA);

	SequencePlayerA->Play();
}

void UCinematicComponent::PlaySequenceB()
{
	OwnerController->SetViewTarget(CineCameraB);

	SequencePlayerB->Play();
}

void UCinematicComponent::StopAllSequence()
{
	if (!(OwnerController->IsLocalController()))
		return;

	SequencePlayerA->Stop();
	SequencePlayerB->Stop();
}

