// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterInputComponent.h"

// Sets default values for this component's properties
URegisterInputComponent::URegisterInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URegisterInputComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!CachAndInit())
		return;

	SetupEnhancedInput();
}

bool URegisterInputComponent::CachAndInit()
{
	PlayerPawn = Cast<APawn>(GetOwner());
	if (!PlayerPawn)
		return false;

	MyController = Cast<APlayerController>(PlayerPawn->GetController());
	if (!MyController)
		return false;

	if (!InputConfig)
	{
		UE_LOG(LogTemp, Log, TEXT("[URegisterInputComponent::CachAndInit] Initialize failed. Please register InputConfig.]"));
		return false;
	}

	return true;
}

void URegisterInputComponent::SetupEnhancedInput()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputConfig->InputMappingContext, 0);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(MyController->InputComponent);
	if (Input)
	{
		Input->BindAction(InputConfig->MyInputAction, ETriggerEvent::Started, this, &URegisterInputComponent::MyBindAction);
	}
}

void URegisterInputComponent::MyBindAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Bind action Success"));
}

