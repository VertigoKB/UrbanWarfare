// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterInputComponent.h"
#include "../../Common/WarfareLogger.h"
#include "../PlayerBase.h"

// Sets default values for this component's properties
URegisterInputComponent::URegisterInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

void URegisterInputComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TestAutoMove)
	{
		TempMove(FVector2D(1.f, 0.f));
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
	PlayerPawn = Cast<APlayerBase>(GetOwner());
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

	if (!PlayerPawn->IsLocallyControlled())
		return false;

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
		Input->BindAction(InputConfig->Movement, ETriggerEvent::Completed, this, &URegisterInputComponent::CompleteMove);
		Input->BindAction(InputConfig->Look, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputLook);
		Input->BindAction(InputConfig->Crouch, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputCrouch);
		Input->BindAction(InputConfig->Walk, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputWalk);
		Input->BindAction(InputConfig->Jump, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputJump);
		Input->BindAction(InputConfig->Test, ETriggerEvent::Started, this, &URegisterInputComponent::InputTest);
	}
}

void URegisterInputComponent::InputMove(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	TempMove(Input);
}

void URegisterInputComponent::TempMove(FVector2D Input)
{
	FVector ForwardVector = PlayerPawn->GetActorForwardVector();
	FVector RightVector = PlayerPawn->GetActorRightVector();

	PlayerPawn->AddMovementInput(ForwardVector, Input.X);
	PlayerPawn->AddMovementInput(RightVector, Input.Y);

	float MinimumVelocity = 250.f;
	FVector PlayerVerlocity = PlayerPawn->GetVelocity();

	OnFootStepPlay.ExecuteIfBound();
}

void URegisterInputComponent::CompleteMove(const FInputActionValue& Value)
{
	OnStopPlayingFootStep.ExecuteIfBound();
}

void URegisterInputComponent::InputLook(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	PlayerPawn->AddControllerYawInput(Input.X);
	PlayerPawn->AddControllerPitchInput(-Input.Y);

	OnInputLook.ExecuteIfBound();
}

void URegisterInputComponent::InputCrouch(const FInputActionValue& Value)
{
	bool Input = Value.Get<bool>();

	OnInputCrouch.ExecuteIfBound(Input);
}

void URegisterInputComponent::InputWalk(const FInputActionValue& Value)
{
	bool Input = Value.Get<bool>();

	OnInputWalk.ExecuteIfBound(Input);
}

void URegisterInputComponent::InputJump(const FInputActionValue& Value)
{
	bool Input = Value.Get<bool>();

	if(!PlayerPawn->IsPlayerFalling())
	{
		OnInputJump.ExecuteIfBound(Input);
	}
}

void URegisterInputComponent::InputTest(const FInputActionValue& Value)
{
	OnTestInput.ExecuteIfBound();
	TempTest();
}

void URegisterInputComponent::TempTest()
{
	TestAutoMove = true;
}