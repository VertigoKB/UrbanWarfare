#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "UrbanWarfare/Common/WarfareLogger.h"
#include "UrbanWarfare/Weapon/WeaponData/WeaponDataAsset.h"
#include "SerializedWeaponInventory.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 WeaponId = 0;

	UPROPERTY()
	uint16 AmmoInMag = 0;

	UPROPERTY()
	uint16 ExtraAmmo = 0;

	bool operator==(const FInventoryItem& Other) const
	{
		return WeaponId == Other.WeaponId;
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

	void SetItem(uint8 Index, uint8 TargetWeaponId, uint16 InAmmoInMag = 0, uint16 InExtraAmmo = 0)
	{
		if (!Items.IsValidIndex(Index))
		{
			LOG_SIMPLE(TEXT("FWeaponInventory SetItem failed: Invalid index %d"), Index);
			return;
		}

		if (Items[Index].WeaponId != TargetWeaponId)
		{
			Items[Index].WeaponId = TargetWeaponId;
			Items[Index].AmmoInMag = InAmmoInMag;
			Items[Index].ExtraAmmo = InExtraAmmo;
			MarkItemDirty(Items[Index]);
		}
	}

	//void SetItem(int32 Index, const FInventoryItem& NewItem)
	//{
	//	if (!Items.IsValidIndex(Index))
	//	{
	//		LOG_SIMPLE(TEXT("SetItemByIndex failed: Invalid index %d"), Index);
	//		return;
	//	}

	//	FInventoryItem& ExistingItem = Items[Index];

	//	// ✅ ReplicationID 유지
	//	uint32 OriginalID = ExistingItem.ReplicationID;

	//	Items[Index] = NewItem;
	//	Items[Index].ReplicationID = OriginalID;

	//	// ✅ Dirty 처리
	//	MarkItemDirty(Items[Index]);

	//	LOG_SIMPLE(TEXT("SetItemByIndex: Updated item at %d with WeaponId %d"), Index, NewItem.WeaponId);
	//}

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
