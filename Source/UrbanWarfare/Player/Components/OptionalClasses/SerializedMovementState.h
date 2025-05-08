// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SerializedMovementState.generated.h"

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
				MarkArrayDirty();
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
		return Items.Contains(FMovementStateEntry{ State });
	}

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FMovementStateEntry, FMovementStateArray>(Items, DeltaParms, *this);
	}

};

template<>
struct TStructOpsTypeTraits<FMovementStateArray> : public TStructOpsTypeTraitsBase2<FMovementStateArray>
{
	enum { WithNetDeltaSerializer = true };
};
