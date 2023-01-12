// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

/**
 * 
 */

class UStaticMesh;

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Pickup Info", ShortTooltip = "Data asset used to configure a pickup."))
class THIRDPERSON_API UPickupInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* DisplayMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector MeshOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SphereRadius;

};

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Item Definition", ShortTooltip = "Data asset used to define a item."))
class THIRDPERSON_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Thumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxStack;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPickupInfo* PickupInfo;
	
};

USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Inventory Item", ShortTooltip = "Struct used for InventoryComponent"))
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 InventoryIndex;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Count;
	
	UPROPERTY(BlueprintReadOnly)
	UItemDefinition* ItemDefinition;

	FInventoryItem() {};

	FInventoryItem(int32 InventoryIndex, int32 Count, UItemDefinition* ItemDefinition)
	{
		this->InventoryIndex = InventoryIndex;
		this->Count = Count;
		this->ItemDefinition = ItemDefinition;
	};

	FORCEINLINE bool IsAddable() const
	{
		return ItemDefinition->MaxStack > Count;
	}
	
	bool Add(int32& NewCount);

	FORCEINLINE bool operator==(UItemDefinition* Other) const
	{
		return this->ItemDefinition == Other;
	}
	FORCEINLINE bool operator!=(UItemDefinition* Other) const
	{
		return this->ItemDefinition == Other;
	}
};