// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPerson/EquipmentSystem/Component/EquipmentSlots.h"
#include "ThirdPerson/Item/ItemDefinition.h"
#include "EquipmentItem.generated.h"

/**
 * 
 */

//////////////////////////////////////////////////////////////////////
// Equipment Definition

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Definition", ShortTooltip = "Data asset used to define a equipment."))
class THIRDPERSON_API UEquipmentDefinition : public UItemDefinition
{
	GENERATED_BODY()

public:
	// Todo EquipmentDefinition에서는 ItemDefinition->ItemTag의 meta=(Categories="Item")을 meta=(Categories="Item.Equipment")로 변경할 방법은 없을까?

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EquipmentSlotTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* DisplayMesh;
	
};

//////////////////////////////////////////////////////////////////////
// Equipment Slot

USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Equipment Item", ShortTooltip = "Struct used for EquipmentComponent"))
struct FEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

protected:
	// 멤버 변수
	UPROPERTY(BlueprintReadOnly)
	FName SlotName;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> CategoryTags;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Index = 0;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag SocketTag;

public:
	// 멤버 함수
	FEquipmentSlot(){}
	FEquipmentSlot(const FMainEquipmentSlot& EquipmentSlot, const int32 Index)
	{
		this->SlotName = EquipmentSlot.SlotName;
		this->CategoryTags = EquipmentSlot.CategoryTags;
		this->Index = Index;
		this->SocketTag = EquipmentSlot.SocketTags[Index];
	}

	FEquipmentSlot(const FSubEquipmentSlot& EquipmentSlot, const int32 Index)
	{
		this->SlotName = EquipmentSlot.SlotName;
		this->CategoryTags = EquipmentSlot.CategoryTags;
		this->Index = Index;
		this->SocketTag = EquipmentSlot.SocketTags[Index];
	}

	FORCEINLINE FName GetSlotName() const { return SlotName; }
	FORCEINLINE TArray<FGameplayTag> GetCategoryTags() const { return CategoryTags; }
	FORCEINLINE int32 GetIndex() const { return Index; }
	FORCEINLINE FGameplayTag GetSocketTag() const { return SocketTag; }

	FORCEINLINE bool operator==(const FEquipmentSlot Other) const { return this->SlotName == Other.SlotName && this->Index == Other.Index; }
	FORCEINLINE bool operator!=(const FEquipmentSlot Other) const { return !(this->SlotName == Other.SlotName && this->Index == Other.Index); }
	
	//////////////////////////////////////////////////////////////////////
	
	FORCEINLINE FString GetName() const { return SlotName.ToString() + FString("[") + FString::FromInt(Index) + FString("]"); }
	FORCEINLINE bool IsMain(const FName MainTag) const { return SlotName == MainTag; }
	FORCEINLINE bool Matches(FEquipmentSlot Other) const { return this->SlotName == Other.GetSlotName(); }
	FORCEINLINE bool Contains(FGameplayTag Other) const
	{
		for(FGameplayTag CategoryTag : CategoryTags)
		{
			if(Other.MatchesTagExact(CategoryTag))
			{
				return true;
			}
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////////
// Equipment Item

USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Equipment Item", ShortTooltip = "Struct used for EquipmentComponent"))
struct FEquipmentItem
{
	GENERATED_USTRUCT_BODY()

public:
	// 멤버 변수
	UPROPERTY(BlueprintReadOnly)
	FEquipmentSlot EquipmentSlot;
	
	UPROPERTY(BlueprintReadOnly)
	UEquipmentDefinition* EquipmentDefinition = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* Equipment = nullptr;
	
	// 멤버 함수
	FEquipmentItem() {}
	FEquipmentItem(FEquipmentSlot EquipmentSlot)
	{
		this->EquipmentSlot = EquipmentSlot;
	};
	
	FORCEINLINE bool IsEmpty() const { return EquipmentDefinition == nullptr && !IsSpawned(); }
	FORCEINLINE bool IsSpawned() const { return Equipment != nullptr; }
	
	bool IsAddable(const UEquipmentDefinition* NewEquipment) const;
	bool Add(UEquipmentDefinition* NewEquipment);
	bool SetEquipment(AActor* SpawnedActor);
	void Clear(){ EquipmentDefinition = nullptr; Equipment = nullptr; }

	//Todo EquipmentComponent에 통합시키는 방법도 있을 것 같다
	FORCEINLINE AActor* RemoveEquipment()
	{
		AActor* ActorToDestroy = Equipment;
		Equipment = nullptr;
		return ActorToDestroy;
	}
	
	FORCEINLINE UEquipmentDefinition* RemoveEquipmentDefinition()
	{
		UEquipmentDefinition* OldEquipment = EquipmentDefinition;
		EquipmentDefinition = nullptr;
		return OldEquipment;
	}

	// For TArray.FindByKey
	FORCEINLINE bool operator==(FEquipmentSlot Other) const { return this->EquipmentSlot == Other; }
	FORCEINLINE bool operator!=(FEquipmentSlot Other) const { return this->EquipmentSlot != Other; }

	//FORCEINLINE bool operator==(FName Other) const { return this->EquipmentSlot.GetSlotName() == Other; }
	//FORCEINLINE bool operator!=(FName Other) const { return this->EquipmentSlot.GetSlotName() != Other; }

	// UEquipmentComponent::CheckEquipSlot(UEquipmentDefinition* NewEquipment)
	FORCEINLINE bool operator==(UEquipmentDefinition* Other) const { return this->EquipmentSlot.Contains(Other->EquipmentSlotTag); }
	FORCEINLINE bool operator!=(UEquipmentDefinition* Other) const { return !(this->EquipmentSlot.Contains(Other->EquipmentSlotTag)); }
};
