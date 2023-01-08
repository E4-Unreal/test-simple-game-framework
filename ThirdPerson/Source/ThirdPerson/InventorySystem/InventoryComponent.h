// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThirdPerson/Item/ItemDefinition.h"
#include "InventoryComponent.generated.h"

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	// ¸â¹ö ÇÔ¼ö
	void Init(int32 InventorySlots);
	bool AddItem(class UItemDefinition* ItemDefinition, int32& ItemCount);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxInventorySlots() const { return MaxInventorySlots; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FInventoryItem> GetInventory() const { return Inventory; }
	
protected:
	// ¸â¹ö º¯¼ö
	int32 MaxInventorySlots;
	TArray<FInventoryItem> Inventory;

	// ¸â¹ö ÇÔ¼ö
	FORCEINLINE void SetInventorySlots(int32 InventorySlots) { MaxInventorySlots = InventorySlots; }
	int32 GetEmptyIndex();
	bool FillSameItem(class UItemDefinition* ItemDefinition, int32& ItemCount);
	FORCEINLINE bool IsAddable() const { return Inventory.Num() < MaxInventorySlots; }
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
