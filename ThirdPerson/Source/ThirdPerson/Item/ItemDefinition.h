// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

/**
 * 
 */

class UStaticMesh;

UCLASS()
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

UCLASS()
class THIRDPERSON_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxStack;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPickupInfo* PickupInfo;
	
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

public:
	int32 InventoryIndex;
	int32 Count;
	UItemDefinition* ItemDefinition;
};