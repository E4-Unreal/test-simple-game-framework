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
	
	// ��� �Լ�
	void Init(int32 MaxInventorySlots);
	bool AddItem(class UItemDefinition* ItemDefinition, int32 &ItemCount);

protected:
	// ��� ����
	int32 MaxInventorySlots;
	TArray<FInventoryItem> Inventory;

	// ��� �Լ�
	FORCEINLINE void SetInventorySlots(int32 InventorySlots)
	{
		MaxInventorySlots = InventorySlots;
	}
	bool FindNewIndex(int32& Index);
	bool FillSameItem(class UItemDefinition* ItemDefinition, int32 &ItemCount);
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
