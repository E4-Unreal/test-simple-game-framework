// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "EquipmentSlots.generated.h"

/**
 * 
 */

//////////////////////////////////////////////////////////////////////
// Equipment Socket Tags
USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Main Equipment Slot"))
struct FMainEquipmentSlot
{
	GENERATED_USTRUCT_BODY()
	
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
	FName DisplayName = "DisplayName";
	
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
	
	// Todo 프로젝트에 설정된 GameplayTag에 따라 meta=(Categories="") 커스터마이징 필요
	// Todo FGameplayTagContainer로 변경?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot"))
	TArray<FGameplayTag> All;

	// Todo 프로젝트에 설정된 GameplayTag에 따라 meta=(Categories="") 커스터마이징 필요
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Active.Main"))
	FGameplayTag Primary;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EquipmentSlot | Main")
	FMainEquipmentSlot Slot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EquipmentSlot | Sub", meta=(TitleProperty="{DisplayName}"))
	TArray<FSubEquipmentSlot> SlotList;

public:
	FORCEINLINE TArray<FGameplayTag> GetAll() const { return All; }
	FORCEINLINE FGameplayTag GetPrimary() const { return Primary; }
};