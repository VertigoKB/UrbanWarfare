// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "PlayerSpawnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UPlayerSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSpawnerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RequestSpawnPlayer(ETeam InTeam, class AWarfareController* InController);

	const FTransform& GetSpawnPointTransformWithRandomIndex(const ETeam InTeam) const;

private:
	bool InitConstruct();

	void RestorePlacedPlayerStart();

private:
	UPROPERTY()
	TArray<AActor*> CounterTristArray;

	UPROPERTY()
	TArray<AActor*> TerroristArray;

	bool bInitFlag = false;

	uint8 CounterTristPointsCount = 0;
	uint8 TerroristPointsCount = 0;
		
};
