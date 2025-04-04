#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "SerializedWeaponInventory.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	EWeaponType Type;

	UPROPERTY()
	EWeaponItem Item;


	FInventoryItem()
		: Type(EWeaponType::UnArmed), Item(EWeaponItem::None) {}

	bool operator==(const FInventoryItem& Other) const
	{
		return (Type == Other.Type) && (Item == Other.Item);
	}

	bool operator!=(const FInventoryItem& Other) const
	{
		return !(*this == Other);
	}

	// 정렬에 필요한 연산자
	bool operator<(const FInventoryItem& Other) const
	{
		return static_cast<uint8>(Type) < static_cast<uint8>(Other.Type);
	}

	bool operator>(const FInventoryItem& Other) const
	{
		return static_cast<uint8>(Type) > static_cast<uint8>(Other.Type);
	}

	// 할당 연산자 오버로딩
	FInventoryItem& operator=(const FInventoryItem& Other)
	{
		if (this != &Other) // 자기 자신을 할당하는 경우 방지
		{
			Type = Other.Type;
			Item = Other.Item;

			// Fast Array Serializer가 변경 사항을 감지하도록 설정
			ReplicationID = Other.ReplicationID;
			ReplicationKey = Other.ReplicationKey;
		}
		return *this;
	}

};

USTRUCT(BlueprintType)
struct FWeaponInventory : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventoryItem> Items;

	void AddItem(const FInventoryItem& NewItem)
	{
		Items.Add(NewItem);
		MarkItemDirty(Items.Last());
	}

	void RemoveItem(int32 Index)
	{
		if (Items.IsValidIndex(Index))
		{
			Items.RemoveAt(Index);
			MarkArrayDirty();
		}
	}

	// 서버에서 변경 사항을 클라이언트에 동기화할 때 호출되는 함수
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryItem, FWeaponInventory>(Items, DeltaParms, *this);
	}

};

// 네트워크 복제 활성화 매크로
template<>
struct TStructOpsTypeTraits<FWeaponInventory> : public TStructOpsTypeTraitsBase2<FWeaponInventory>
{
	enum { WithNetDeltaSerializer = true };
};
