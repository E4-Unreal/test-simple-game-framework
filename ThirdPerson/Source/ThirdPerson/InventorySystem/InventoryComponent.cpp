// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ThirdPerson/ThirdPerson.h"
#include "ThirdPerson/Item/ItemDefinition.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::Init(int32 InventorySlots)
{
	if(InventorySlots < 0)
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryComponent::Init\nInventorySlotNum < 0"));
		SetInventorySlots(0);
	}
	else
	{
		UE_LOG(LogInventory, Log, TEXT("InventoryComponent::Init\nInventorySlotNum: %d"), InventorySlots);
		SetInventorySlots(InventorySlots);
	}
}

bool UInventoryComponent::AddItem(UItemDefinition* ItemDefinition, int32& ItemCount)
{
	UE_LOG(LogInventory, Log, TEXT("InventoryComponent::AddItem\nItemDefinition: %s / Count: %d"), *ItemDefinition->GetName(), ItemCount);

	// �Էµ� ������ ��ȿ�� �˻�
	if(ItemDefinition == nullptr || ItemCount <= 0)
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryComponent::AddItem\nItemCount <= 0"));
		return false;
	}

	// ������ ������ ����� ä���ֱ�
	bool bAdded = FillSameItem(ItemDefinition, ItemCount);

	// ���ο� ������ Ȥ�� �뷮�� �ʰ��� ������ �߰�
	while(ItemCount > 0 && IsAddable())
	{
		bAdded = true;
		int32 Count;
		if(ItemCount >= ItemDefinition->MaxStack)
		{
			Count = ItemDefinition->MaxStack;
			ItemCount -= Count;
		}
		else
		{
			Count = ItemCount;
			ItemCount = 0;
		}

		int32 NewIndex = GetEmptyIndex();
		FInventoryItem NewInventoryItem(NewIndex, Count, ItemDefinition);
		Inventory.Add(NewInventoryItem);
		UE_LOG(LogInventory, Log, TEXT("Item Added To New Inventory Slot\nIndex: %d / Count: %d"), NewIndex, Count);
	}
	
	if(bAdded)
	{
		UE_LOG(LogInventory, Log, TEXT("Add Item To Inventory Complete\nItemCount Reaminder: %d"), ItemCount);
	}
	else
	{
		UE_LOG(LogInventory, Log, TEXT("Inventory Is Full"));
	}
	return bAdded;
}

int32 UInventoryComponent::GetEmptyIndex()
{
	// �������� ����ִ� �κ��丮 ���� �ε��� �˻�
	TArray<int32> Indices;
	for(FInventoryItem InventoryItem : Inventory)
	{
		Indices.Add(InventoryItem.InventoryIndex);
	}

	// ��� �ִ� �ε��� �� ���� ���� �ε��� ��ȯs
	for(int i = 0; i < MaxInventorySlots; i++)
	{
		if(Indices.Find(i) == INDEX_NONE)
		{
			return i;
		}
	}
	UE_LOG(LogInventory, Error, TEXT("InventoryComponent::GetEmptyIndex\nNo Empty Index. Use IsAddable() first."));
	return -1;
}

bool UInventoryComponent::FillSameItem(UItemDefinition* ItemDefinition, int32& ItemCount)
{
	// �κ��丮�� ������ ������ �˻� �� �뷮�� �� �� �����۵鸸 �ε��� ���
	TArray<int32> Indices;
	for(FInventoryItem InventoryItem : Inventory)
	{
		if(InventoryItem == ItemDefinition && InventoryItem.IsAddable())
		{
			Indices.Add(InventoryItem.InventoryIndex);
		}
	}

	// �� ���̶� ä���ٸ� true ��ȯ
	if(Indices.IsEmpty())
	{
		return false;
	}
	else
	{
		UE_LOG(LogInventory, Log, TEXT("InventoryComponent::FillSameItem"));
		Indices.Sort();

		// ���ʴ�� ������ ���� �߰�
		for(const int32 Index : Indices)
		{
			// �Է� ������ ���� : ItemCount <= 0
			if(!Inventory[Index].Add(ItemCount))
			{
				break;
			}
		}
		return true;
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

