// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterInputComponent.h"
#include "UrbanWarfare/Common/WarfareLogger.h"
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

	SetComponentTickEnabled(false);

	GetWorld()->GetTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([this]() {

		bInitFlag = CachAndInit();

		if (bInitFlag)
		{
			SetComponentTickEnabled(true);
			SetupEnhancedInput();
			GetWorld()->GetTimerManager().ClearTimer(InitTimer);
		}
		else
		{
			//LOG_EFUNC(TEXT("Failed to initialize. Count: %d"), InitCount);
			InitCount++;

			if (InitCount > 10)
			{
				//LOG_EFUNC(TEXT("Unable to initialize. Process end."));
				GetWorld()->GetTimerManager().ClearTimer(InitTimer);
			}
		}

		}), 0.5f, true);
}

void URegisterInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(InitTimer);
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
	{
		InitCount = 10;
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
		Input->BindAction(InputConfig->Movement, ETriggerEvent::Completed, this, &URegisterInputComponent::CompleteMove);
		Input->BindAction(InputConfig->Look, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputLook);
		Input->BindAction(InputConfig->Crouch, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputCrouch);
		Input->BindAction(InputConfig->Walk, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputWalk);
		Input->BindAction(InputConfig->Jump, ETriggerEvent::Triggered, this, &URegisterInputComponent::InputJump);
		Input->BindAction(InputConfig->EquipRifle, ETriggerEvent::Started, this, &URegisterInputComponent::InputEquipRifle);
		Input->BindAction(InputConfig->EquipPistol, ETriggerEvent::Started, this, &URegisterInputComponent::InputEquipPistol);
		Input->BindAction(InputConfig->ThrowWeapon, ETriggerEvent::Started, this, &URegisterInputComponent::InputThrowWeapon);
		Input->BindAction(InputConfig->Attack, ETriggerEvent::Started, this, &URegisterInputComponent::InputAttack);
		Input->BindAction(InputConfig->Attack, ETriggerEvent::Completed, this, &URegisterInputComponent::InputCompleteAttack);
		Input->BindAction(InputConfig->Reload, ETriggerEvent::Started, this, &URegisterInputComponent::InputReload);
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

	//OnFootStepPlay.ExecuteIfBound();
}

void URegisterInputComponent::CompleteMove(const FInputActionValue& Value)
{
	//OnStopPlayingFootStep.ExecuteIfBound();
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

void URegisterInputComponent::InputEquipRifle(const FInputActionValue& Value)
{
	//LOG_SIMPLE(TEXT("OnEquipRifle"));
	OnInputEquipRifle.ExecuteIfBound();
}

void URegisterInputComponent::InputEquipPistol(const FInputActionValue& Value)
{
	//LOG_SIMPLE(TEXT("OnEquipPistol"));
	OnInputEquipPistol.ExecuteIfBound();
}

void URegisterInputComponent::InputThrowWeapon(const FInputActionValue& Value)
{
	OnThrowWeapon.ExecuteIfBound();
}

void URegisterInputComponent::InputAttack(const FInputActionValue& Value)
{
	OnInputAttack.ExecuteIfBound();
}

void URegisterInputComponent::InputCompleteAttack(const FInputActionValue& Value)
{
	OnCompleteAttack.ExecuteIfBound();
}

void URegisterInputComponent::InputReload(const FInputActionValue& Value)
{
	OnInputReload.ExecuteIfBound();
}

void URegisterInputComponent::TempTest()
{
	TestAutoMove = true;
}