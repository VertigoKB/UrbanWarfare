// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CinematicComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UCinematicComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCinematicComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<AActor*> SceneCameras;
	UPROPERTY()
	TArray<AActor*> SequenceActors;

	bool CameraNumber = 0;
		
private:
	UPROPERTY()
	TObjectPtr<class ACineCameraActor> CineCameraA;

	UPROPERTY()
	TObjectPtr<class ACineCameraActor> CineCameraB;

	UPROPERTY()
	TObjectPtr<class ALevelSequenceActor> SceneA;

	UPROPERTY()
	TObjectPtr<class ALevelSequenceActor> SceneB;
};
