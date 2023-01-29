// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ThirdPerson/ThirdPerson.h"
#include "ItemDefinition.generated.h"

/**
 * 
 */

//////////////////////////////////////////////////////////////////////
// Pickup Info

class UStaticMesh;
class AActor;

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Pickup Info", ShortTooltip = "Data asset used to configure a pickup."))
class THIRDPERSON_API UPickupInfo : public UDataAsset
{
	GENERATED_BODY()

	// 멤버 변수
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* DisplayMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeshOffset")
	FVector Location;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeshOffset")
	FRotator Rotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SphereRadius;

	// 멤버 함수
public:
	FORCEINLINE bool CheckDisplayMesh() const
	{
		if(!IsValid(DisplayMesh)){ UE_LOG(LogItem, Warning, TEXT("%s::CheckDisplayMesh > DisplayMesh is not valid"), *this->GetName()) return false; }
		return true;
	}
	
};

//////////////////////////////////////////////////////////////////////
// Item Definition

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Item Definition", ShortTooltip = "Data asset used to define a item."))
class THIRDPERSON_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()

	// 멤버 변수
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	// Todo C++이 아니라 에디터에서 설정할 수 있는 게임플레이 태그를 제한하는 방법은 없을까?
	// Todo 프로젝트에 설정된 GameplayTag에 따라 meta=(Categories="") 커스터마이징 필요
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="Item"))
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Thumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxStack = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPickupInfo* PickupInfo;

	// 멤버 함수
public:
	FORCEINLINE bool CheckThumbnail() const
	{
		if(!IsValid(Thumbnail)){ UE_LOG(LogItem, Warning, TEXT("%s::CheckThumbnail > Thumbnail is not valid"), *this->GetName()) return false; }
		return true;
	}
	FORCEINLINE bool CheckPickupInfo() const
	{
		if(!IsValid(PickupInfo)){ UE_LOG(LogItem, Warning, TEXT("%s::CheckPickupInfo > PickupInfo is not valid"), *this->GetName()) return false; }
		return true;
	}
};

//////////////////////////////////////////////////////////////////////
// Inventory Item

USTRUCT(Atomic, BlueprintType, Meta = (DisplayName = "Inventory Item", ShortTooltip = "Struct used for InventoryComponent"))
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

public:
	// 멤버 변수
	UPROPERTY(BlueprintReadOnly)
	int32 InventoryIndex;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Count;
	
	UPROPERTY(BlueprintReadOnly)
	UItemDefinition* ItemDefinition;

	// 멤버 함수
	FInventoryItem() {};

	FInventoryItem(int32 InventoryIndex, int32 Count, UItemDefinition* ItemDefinition)
	{
		this->InventoryIndex = InventoryIndex;
		this->Count = Count;
		this->ItemDefinition = ItemDefinition;
	};

	bool Add(int32& NewCount);

	FORCEINLINE bool IsAddable() const { return ItemDefinition->MaxStack > Count; }

	// For TArray.Remove
	FORCEINLINE bool operator==(FInventoryItem Other) const { return this->InventoryIndex == Other.InventoryIndex; }
	FORCEINLINE bool operator!=(FInventoryItem Other) const { return this->InventoryIndex != Other.InventoryIndex; }

	// For Searching Item in Inventory
	FORCEINLINE bool operator==(UItemDefinition* Other) const { return this->ItemDefinition == Other; }
	FORCEINLINE bool operator!=(UItemDefinition* Other) const { return this->ItemDefinition != Other; }

	// For TArray.FindByKey
	FORCEINLINE bool operator==(int32 Index) const { return this->InventoryIndex == Index; }
	FORCEINLINE bool operator!=(int32 Index) const { return this->InventoryIndex != Index; }
};