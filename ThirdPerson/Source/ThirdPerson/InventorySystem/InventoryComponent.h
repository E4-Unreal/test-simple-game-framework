// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThirdPerson/Item/ItemDefinition.h"
#include "InventoryComponent.generated.h"

// �̺�Ʈ ����ó ��ũ��
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryDelegate);

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	// ��� �Լ�
	void Init(int32 InventorySlots);
	bool AddItem(class UItemDefinition* ItemDefinition, int32& ItemCount);

	UFUNCTION(BlueprintCallable)
	bool SwapItems(int32 SourceIndex, int32 DestinationIndex);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxInventorySlots() const { return MaxInventorySlots; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FInventoryItem> GetInventory() const { return Inventory; }

	// �̺�Ʈ ����ó
	UPROPERTY(BlueprintAssignable, Category = "Test")
	FInventoryDelegate OnUpdate;
	
protected:
	// ��� ����
	int32 MaxInventorySlots;
	TArray<FInventoryItem> Inventory;

	// ��� �Լ�
	FORCEINLINE void SetInventorySlots(int32 InventorySlots) { MaxInventorySlots = InventorySlots; }
	int32 GetEmptyIndex();
	bool FillSameItem(class UItemDefinition* ItemDefinition, int32& ItemCount);
	FORCEINLINE bool IsAddable() const { return Inventory.Num() < MaxInventorySlots; }
	FORCEINLINE FInventoryItem* GetInventoryItemByIndex(int32 Index) { return Inventory.FindByKey(Index); }
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
