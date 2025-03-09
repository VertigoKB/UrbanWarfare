// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterInputComponent.h"
#include "../AssetConfig/InputConfig.h"
#include "../Common/ErrorLogger.h"
#include "PlayerBase.h"

// Sets default values for this component's properties
URegisterInputComponent::URegisterInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	if (!InputConfig)
	{
		ConstructorHelpers::FObjectFinder<UInputConfig> DefaultConfig(TEXT("/Game/Blueprints/AssetConfigs/InputConfig.InputConfig"));
		if (DefaultConfig.Succeeded())
		{
			InputConfig = DefaultConfig.Object;
			bInputConfigChecker = true;
		}
		else
			LOG_NULL(InputConfig);
	}


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
		LOG_NULL(InputConfig);
		return false;
	}
	return true;
}

void URegisterInputComponent::SetupEnhancedInput()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputConfig->DefaultMappingContext, 0);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(MyController->InputComponent);
	if (Input)
	{
		Input->BindAction(InputConfig->Movement, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputMove);
		Input->BindAction(InputConfig->Look, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputLook);
		Input->BindAction(InputConfig->Crouch, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputCrouch);
	}
}

void URegisterInputComponent::InputMove(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	FVector ForwardVector = PlayerPawn->GetActorForwardVector();
	FVector RightVector = PlayerPawn->GetActorRightVector();

	PlayerPawn->AddMovementInput(ForwardVector, Input.X);
	PlayerPawn->AddMovementInput(RightVector, Input.Y);
}

void URegisterInputComponent::InputLook(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	PlayerPawn->AddControllerYawInput(Input.X);
	PlayerPawn->AddControllerPitchInput(-Input.Y);
}

void URegisterInputComponent::InputCrouch(const FInputActionValue& Value)
{
	bool Input = Value.Get<bool>();

	OnInputCrouch.ExecuteIfBound(Input);
}
