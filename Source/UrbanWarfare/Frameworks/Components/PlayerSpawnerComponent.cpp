// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpawnerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

// Sets default values for this component's properties
UPlayerSpawnerComponent::UPlayerSpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CounterTristArray.Reserve(4);
	TerroristArray.Reserve(4);
}


// Called when the game starts
void UPlayerSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	bInitFlag = InitConstruct();
}


// Called every frame
void UPlayerSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerSpawnerComponent::RequestSpawnPlayer(ETeam InTeam, AWarfareController* InController)
{
	switch (InTeam)
	{
	case ETeam::CounterTrist:
	{
		uint8 Index = FMath::RandRange(0, CounterTristPointsCount - 1);
		InController->SpawnPlayer(ETeam::CounterTrist, CounterTristArray[Index]->GetActorTransform());
		break;
	}
	case ETeam::Terrorist:
	{
		uint8 Index = FMath::RandRange(0, TerroristPointsCount - 1);
		InController->SpawnPlayer(ETeam::Terrorist, TerroristArray[Index]->GetActorTransform());
		break;
	}
	}

}

const FTransform& UPlayerSpawnerComponent::GetSpawnPointTransformWithRandomIndex(const ETeam InTeam) const
{ 
	if (!bInitFlag)
	{
		//LOG_EFUNC(TEXT("초기화가 완료되지 않아 Identity Transform이 반환됨"));
		return FTransform::Identity;
	}

	switch (InTeam)
	{
	case ETeam::CounterTrist:
	{
		uint8 Index = FMath::RandRange(0, CounterTristPointsCount - 1);
		return CounterTristArray[Index]->GetActorTransform();
		break;
	}
	case ETeam::Terrorist:
	{
		uint8 Index = FMath::RandRange(0, TerroristPointsCount - 1);
		return TerroristArray[Index]->GetActorTransform();
		break;
	}
	}

	//LOG_EFUNC(TEXT("예외가 발생하여 Identity Transform이 반환됨"));
	return FTransform::Identity;

}

bool UPlayerSpawnerComponent::InitConstruct()
{
	RestorePlacedPlayerStart();

	CounterTristPointsCount = CounterTristArray.Num();
	TerroristPointsCount = TerroristArray.Num();

	if ((CounterTristPointsCount > 0) && 
		(TerroristPointsCount > 0))
		return true;
	else
		return false;
}

void UPlayerSpawnerComponent::RestorePlacedPlayerStart()
{
	TArray<AActor*> PlayerStartFinder;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartFinder);

	for (const auto& Iter : PlayerStartFinder)
	{
		if (Iter->Tags.Contains("CounterTrist"))
			CounterTristArray.Add(Iter);
		else if (Iter->Tags.Contains("Terrorist"))
			TerroristArray.Add(Iter);
	}
}

