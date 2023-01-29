// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
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
// Equipment Item

class AEquipment;

USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Equipment Item", ShortTooltip = "Struct used for EquipmentComponent"))
struct FEquipmentItem
{
	GENERATED_USTRUCT_BODY()

public:
	// 멤버 변수
	UPROPERTY(BlueprintReadOnly)
	FEquipmentSlot EquipmentSlot;

	// Todo Change to EquipmentInstance (UEquipmentDefinition, FEquipmentStat)
	UPROPERTY(BlueprintReadOnly)
	UEquipmentDefinition* EquipmentDefinition = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	AEquipment* Equipment = nullptr;
	
	// 멤버 함수
	FEquipmentItem() {}
	FEquipmentItem(FEquipmentSlot EquipmentSlot)
	{
		this->EquipmentSlot = EquipmentSlot;
	};
	
	FORCEINLINE bool IsEmpty() const { return EquipmentDefinition == nullptr && !IsSpawned(); }
	FORCEINLINE bool IsSpawned() const { return Equipment != nullptr; }
	
	bool IsAddable(const UEquipmentDefinition* NewEquipment) const;
	bool SetEquipmentDefinition(UEquipmentDefinition* NewEquipment);
	bool SetEquipment(AEquipment* SpawnedActor);
	void Clear()
	{
		Equipment->Destroy();
		Equipment = nullptr;
		EquipmentDefinition = nullptr;
	}

	// For EquipmentSlot
	FORCEINLINE bool IsMainSlot() const { return EquipmentSlot.IsMainSlot(); }
	FORCEINLINE bool IsActiveSlot() const { return EquipmentSlot.IsActiveSlot(); }
	FORCEINLINE bool IsPassiveSlot() const { return EquipmentSlot.IsPassiveSlot(); }

	//Todo EquipmentComponent에 통합시키는 방법도 있을 것 같다
	FORCEINLINE AEquipment* RemoveEquipment()
	{
		AEquipment* ActorToDestroy = Equipment;
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

	// UEquipmentComponent::CheckEquipSlot(UEquipmentDefinition* NewEquipment)
	FORCEINLINE bool operator==(UEquipmentDefinition* Other) const { return this->EquipmentSlot.MatchesTag(Other->EquipmentSlotTag); }
	FORCEINLINE bool operator!=(UEquipmentDefinition* Other) const { return !(this->EquipmentSlot.MatchesTag(Other->EquipmentSlotTag)); }
};
