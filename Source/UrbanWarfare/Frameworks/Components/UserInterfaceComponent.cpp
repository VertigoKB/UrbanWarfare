// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaceComponent.h"
#include "Components/Button.h"

#include "UrbanWarfare/UI/MainMenu.h"
#include "UrbanWarfare/Common/WarfareLogger.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Frameworks/WarfareHud.h"
#include "UrbanWarfare/Frameworks/WarfarePlayerState.h"
#include "UrbanWarfare/Frameworks/WarfareGameMode.h"


UUserInterfaceComponent::UUserInterfaceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUserInterfaceComponent::BeginPlay()
{
	Super::BeginPlay();


	GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([this]() {

		if (InitConstruct() || InitCount <= 0)
			GetWorld()->GetTimerManager().ClearTimer(InitTimer);
		else
			InitCount--;

		}), 0.05f, true);
}

void UUserInterfaceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

bool UUserInterfaceComponent::InitConstruct()
{
	MyOwner = GetOwner<AWarfareController>();
	if (!MyOwner)
		return false;

	//if (!MyOwner->IsLocalController())
	//	return false;

	OwnerPlayerState = MyOwner->GetPlayerState<AWarfarePlayerState>();
	if (!OwnerPlayerState)
		return false;

	MyHud = MyOwner->GetHUD<AWarfareHud>();
	if (!MyHud)
		return false;

	MainMenu = MyHud->GetWidgetMainMenu();
	if (!MainMenu)
		return false;

	DebugBoolA = true;
	return true;
}

void UUserInterfaceComponent::ServerRequestSpawnPlayer_Implementation()
{
	if (!OwnerPlayerState)
		return;

	GetWorld()->GetAuthGameMode<AWarfareGameMode>()->SpawnPlayerByPlayerState(OwnerPlayerState);
}

void UUserInterfaceComponent::OnReleaseGameStart()
{
	if (!MyOwner->HasAuthority() ||
		GetWorld()->GetNetMode() == NM_ListenServer)
	{
		MainMenu->SetVisibility(ESlateVisibility::Collapsed);
		//MainMenu->RemoveFromParent();

		FInputModeGameOnly InputMode;
		MyOwner->SetInputMode(InputMode);

		ServerRequestSpawnPlayer();
	}


}

// Called every frame
//void UUserInterfaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}
