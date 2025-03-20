// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "PlayerBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Running = 0,
	Walking,
	Crouching
};

USTRUCT(BlueprintType)
struct FMovementStateEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EMovementState State;

	FMovementStateEntry() : State(EMovementState::Running) {}
	FMovementStateEntry(EMovementState InState) : State(InState) {}

	bool operator==(const FMovementStateEntry& Other) const
	{
		return State == Other.State;
	}
};

USTRUCT(BlueprintType)
struct FMovementStateArray : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FMovementStateEntry> Items;
	void AddState(EMovementState NewState)
	{
		for (const auto& Entry : Items)
		{
			if (Entry.State == NewState)
			{
				return;
			}
		}

		FMovementStateEntry NewEntry;
		NewEntry.State = NewState;
		Items.Add(NewEntry);

		MarkItemDirty(NewEntry);
	}

	void RemoveState(EMovementState StateToRemove)
	{
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (Items[i].State == StateToRemove)
			{
				Items.RemoveAt(i);
				MarkArrayDirty(); // 배열 변경 감지
				break;
			}
		}
	}

	EMovementState LastState() const
	{
		return Items.Last().State;
	}

	void Reserve(int32 Number)
	{
		Items.Reserve(Number);
	}

	bool ContainState(EMovementState State) const
	{
		return Items.Contains(FMovementStateEntry{State});
	}

	// 서버에서 변경 사항을 클라이언트에 동기화할 때 호출되는 함수
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FMovementStateEntry, FMovementStateArray>(Items, DeltaParms, *this);
	}

};

// 네트워크 복제 활성화 매크로
template<>
struct TStructOpsTypeTraits<FMovementStateArray> : public TStructOpsTypeTraitsBase2<FMovementStateArray>
{
	enum { WithNetDeltaSerializer = true };
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UPlayerBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBehaviorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UFUNCTION()
	bool InitConstruct();

protected:
	UFUNCTION()
	void OnRep_MovementState();

	UFUNCTION(Server, Reliable)
	void ServerApplySpeed();

protected:
	// Crouch

	UFUNCTION()
	void TriggerCrouch(bool bCrouch);

	UFUNCTION(Server, Reliable)
	void ServerCrouch(bool bCrouch);

protected:
	// Walk

	UFUNCTION()
	void TriggerWalk(bool bWalk);

	UFUNCTION(Server, Unreliable)
	void ServerWalk(bool bWalk);

protected:
	// Jump

	UFUNCTION()
	void TriggerJump(bool Jump);

protected:
	// Look
	UFUNCTION()
	void TriggerLook();

	UFUNCTION(Server, Unreliable)
	void ServerLook();

	UFUNCTION()
	void VerifyNecessitySyncAimDirection();
	
	UFUNCTION()
	uint8 CalcAimDirection();

	inline float DecompressePitch(uint8 CompressedPitch) {
		return (CompressedPitch / 255.f) * 180.f - 90.f;
	}
	
protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> ThePlayer;

	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegInputComp;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> PlayerMovement;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Constant")
	float WalkSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Constant")
	float RunSpeed = 750.f;

	UPROPERTY(Replicated)
	float CurrentSpeed = RunSpeed;

public:
	UPROPERTY(ReplicatedUsing = OnRep_MovementState)
	FMovementStateArray MovementState;

	UPROPERTY(Replicated)
	float AimDirection = 0.f;

private:
	FTimerHandle SyncAimTimer;

	FTimerHandle FallingChecker;

	bool bIsInitialized = false;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 DebugA;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 DebugB;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DebugF;
};
