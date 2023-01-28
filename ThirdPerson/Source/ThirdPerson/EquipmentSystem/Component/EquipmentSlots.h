// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ThirdPerson/GamePlayTags/ThirdPersonGameplayTags.h"
#include "EquipmentSlots.generated.h"

/**
 * 
 */

// Todo FGameplayTag Categories를 변수화하는 방법은? 하나의 구조체로 대체하고 싶다
//////////////////////////////////////////////////////////////////////
// Main Equipment Slot
USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Main Equipment Slot"))
struct FMainEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Active.Main"))
	FGameplayTag Category = FThirdPersonGameplayTags::Get().EquipmentSlot_Active_Main;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SlotName = "Main";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Active.Main"))
	TArray<FGameplayTag> FilterCategories;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSocket"))
	TArray<FGameplayTag> Sockets;
	
};

//////////////////////////////////////////////////////////////////////
// Sub Equipment Slot
USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Sub Equipment Slot"))
struct FSubEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Active.Sub"))
	FGameplayTag Category = FThirdPersonGameplayTags::Get().EquipmentSlot_Active_Sub;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SlotName = "Sub";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Active.Sub"))
	TArray<FGameplayTag> FilterCategories;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSocket"))
	TArray<FGameplayTag> Sockets;
};

//////////////////////////////////////////////////////////////////////
// Passive Equipment Slot
USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Passive Equipment Slot"))
struct FPassiveEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Passive"))
	FGameplayTag Category = FThirdPersonGameplayTags::Get().EquipmentSlot_Passive;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SlotName = "Passive";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Passive"))
	TArray<FGameplayTag> FilterCategories;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSocket"))
	TArray<FGameplayTag> Sockets;
};

//////////////////////////////////////////////////////////////////////
// Equipment Slot

USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Equipment Slot", ShortTooltip = "Struct used for EquipmentComponent"))
struct FEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

protected:
	// 멤버 변수
	UPROPERTY()
	FGameplayTag Category;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index = 0;
	
	UPROPERTY()
	TArray<FGameplayTag> FilterCategories;

	UPROPERTY()
	FGameplayTag Socket;

public:
	// 멤버 함수
	FEquipmentSlot(){}
	FEquipmentSlot(const FMainEquipmentSlot& EquipmentSlot, const int32 Index)
	{
		this->Category = EquipmentSlot.Category;
		this->SlotName = EquipmentSlot.SlotName;
		this->Index = Index;
		this->FilterCategories = EquipmentSlot.FilterCategories;
		this->Socket = EquipmentSlot.Sockets[Index];
	}

	FEquipmentSlot(const FSubEquipmentSlot& EquipmentSlot, const int32 Index)
	{
		this->Category = EquipmentSlot.Category;
		this->SlotName = EquipmentSlot.SlotName;
		this->Index = Index;
		this->FilterCategories = EquipmentSlot.FilterCategories;
		this->Socket = EquipmentSlot.Sockets[Index];
	}

	FEquipmentSlot(const FPassiveEquipmentSlot& EquipmentSlot, const int32 Index)
	{
		this->Category = EquipmentSlot.Category;
		this->SlotName = EquipmentSlot.SlotName;
		this->Index = Index;
		this->FilterCategories = EquipmentSlot.FilterCategories;
		this->Socket = EquipmentSlot.Sockets[Index];
	}

	FEquipmentSlot(FName SlotName, int32 Index)
	{
		this->SlotName = SlotName;
		this->Index = Index;
	}

	FORCEINLINE FGameplayTag GetCategory() const { return Category; }
	FORCEINLINE FName GetSlotName() const { return SlotName; }
	FORCEINLINE TArray<FGameplayTag> GetFilterCategories() const { return FilterCategories; }
	FORCEINLINE int32 GetIndex() const { return Index; }
	FORCEINLINE FGameplayTag GetSocket() const { return Socket; }

	FORCEINLINE bool IsMainSlot() const { return Category.MatchesTagExact(FThirdPersonGameplayTags::Get().EquipmentSlot_Active_Main); }
	FORCEINLINE bool IsActiveSlot() const { return Category.MatchesTag(FThirdPersonGameplayTags::Get().EquipmentSlot_Active); }
	FORCEINLINE bool IsPassiveSlot() const { return Category.MatchesTag(FThirdPersonGameplayTags::Get().EquipmentSlot_Passive); }

	FORCEINLINE bool operator==(const FEquipmentSlot Other) const { return this->SlotName == Other.SlotName && this->Index == Other.Index; }
	FORCEINLINE bool operator!=(const FEquipmentSlot Other) const { return !(this->SlotName == Other.SlotName && this->Index == Other.Index); }
	
	//////////////////////////////////////////////////////////////////////
	
	FORCEINLINE FString GetName() const { return SlotName.ToString() + FString("[") + FString::FromInt(Index) + FString("]"); }
	FORCEINLINE bool MatchesSlotName(FEquipmentSlot Other) const { return this->SlotName == Other.GetSlotName(); }
	FORCEINLINE bool MatchesTag(FGameplayTag Other) const
	{
		// 기본 카테고리 확인
		if(!Other.MatchesTag(Category)){ return false; }

		// 기본 카테고리와 동일하면 필터링 생략
		if(Other.MatchesTagExact(Category)){ return true; }

		// 필터링 카테고리 0개면 모든 카테고리 장착 가능
		if(FilterCategories.Num() == 0){ return true; }

		// 필터링 카테고리 확인
		for(FGameplayTag FilterCategory : FilterCategories)
		{
			if(Other.MatchesTagExact(FilterCategory))
			{
				return true;
			}
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////////
// Equipment Slots

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Slots", ShortTooltip = "DataAsset for mapping Equipment Slot GameplayTags to Equipment Socket GameplayTags"))
class THIRDPERSON_API UEquipmentSlots : public UDataAsset
{
	GENERATED_BODY()

protected:
	// Todo C++이 아니라 에디터에서 설정할 수 있는 게임플레이 태그를 제한하는 방법은 없을까?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EquipmentSlots|Active|Main")
	FMainEquipmentSlot MainSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EquipmentSlots|Active|Sub", meta=(TitleProperty="{SlotName}"))
	TArray<FSubEquipmentSlot> SubSlotList;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EquipmentSlots|Passive", meta=(TitleProperty="{SlotName}"))
	TArray<FPassiveEquipmentSlot> PassiveSlotList;

public:
	FORCEINLINE FMainEquipmentSlot GetMainEquipmentSlot() const { return MainSlot; }
	FORCEINLINE TArray<FSubEquipmentSlot> GetSubEquipmentSlotList() const { return SubSlotList; }
	FORCEINLINE TArray<FPassiveEquipmentSlot> GetPassiveEquipmentSlotList() const { return PassiveSlotList; }
};