// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
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

USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Equipment Item", ShortTooltip = "Struct used for EquipmentComponent"))
struct FEquipmentItem
{
	GENERATED_USTRUCT_BODY()

public:
	// 멤버 변수
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag EquipmentSlot;
	
	UPROPERTY(BlueprintReadOnly)
	UEquipmentDefinition* EquipmentDefinition = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* Equipment = nullptr;
	
	// 멤버 함수
	FEquipmentItem() {}
	FEquipmentItem(FGameplayTag EquipmentSlot)
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
	FORCEINLINE bool operator==(FGameplayTag Other) const { return this->EquipmentSlot.MatchesTagExact(Other); }
	FORCEINLINE bool operator!=(FGameplayTag Other) const { return !this->EquipmentSlot.MatchesTagExact(Other); }
};
