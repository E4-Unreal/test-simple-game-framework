// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "EquipmentSlots.generated.h"

/**
 * 
 */

// Todo FGameplayTag Categories를 변수화하는 방법은?
//////////////////////////////////////////////////////////////////////
// Equipment Socket Tags
USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Main Equipment Slot"))
struct FMainEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FName SlotName = "Main";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Active.Main"))
	TArray<FGameplayTag> CategoryTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSocket"))
	TArray<FGameplayTag> SocketTags;
};

//////////////////////////////////////////////////////////////////////
// Equipment Socket Tags
USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Sub Equipment Slot"))
struct FSubEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SlotName = "SlotName";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Active.Sub"))
	TArray<FGameplayTag> CategoryTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSocket"))
	TArray<FGameplayTag> SocketTags;
};

//////////////////////////////////////////////////////////////////////
// Equipment Slots

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Slot Tags", ShortTooltip = "Equipment SlotTags for Equipment Slots"))
class THIRDPERSON_API UEquipmentSlots : public UDataAsset
{
	GENERATED_BODY()

protected:
	// Todo C++이 아니라 에디터에서 설정할 수 있는 게임플레이 태그를 제한하는 방법은 없을까?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EquipmentSlots | Main")
	FMainEquipmentSlot Slot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EquipmentSlots | Sub", meta=(TitleProperty="{SlotName}"))
	TArray<FSubEquipmentSlot> SlotList;

public:
	FORCEINLINE FMainEquipmentSlot GetMainEquipmentSlot() const { return Slot; }
	FORCEINLINE TArray<FSubEquipmentSlot> GetSubEquipmentSlot() const { return SlotList; }
};