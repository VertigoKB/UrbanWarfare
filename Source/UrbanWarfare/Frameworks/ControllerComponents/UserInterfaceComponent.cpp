// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaceComponent.h"
#include "Components/Button.h"
#include "UrbanWarfare/Frameworks/WarfareController.h"
#include "UrbanWarfare/Frameworks/WarfareHud.h"
#include "UrbanWarfare/Frameworks/WarfarePlayerState.h"
#include "UrbanWarfare/UI/MainMenu.h"
#include "UrbanWarfare/Common/WarfareLogger.h"


UUserInterfaceComponent::UUserInterfaceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUserInterfaceComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([this]() {
		bInitFlag = InitConstruct();

		FInputModeUIOnly InputMode;
		MyOwner->SetInputMode(InputMode);

		if (bInitFlag)
		{
			if (MainMenu->ButtonGameStart)
				MainMenu->ButtonGameStart->OnClicked.AddDynamic(this, &UUserInterfaceComponent::OnClickedGameStart);
		}

		}), 0.5f, false);
}

void UUserInterfaceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(InitTimer);
}

bool UUserInterfaceComponent::InitConstruct()
{
	MyOwner = GetOwner<AWarfareController>();
	if (!MyOwner)
		return false;

	MyHud = MyOwner->GetHUD<AWarfareHud>();
	if (!MyHud)
		return false;

	MainMenu = MyHud->GetWidgetMainMenu();
	if (!MainMenu)
		return false;

	return true;
}

void UUserInterfaceComponent::OnClickedGameStart()
{
	MainMenu->SetVisibility(ESlateVisibility::Collapsed);
	//MainMenu->RemoveFromParent();

	FInputModeGameOnly InputMode;
	MyOwner->SetInputMode(InputMode);

	MyOwner->GetPlayerState<AWarfarePlayerState>()->OnClickedGameStart();

}

// Called every frame
//void UUserInterfaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}
