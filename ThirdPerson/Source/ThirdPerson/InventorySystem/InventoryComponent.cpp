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

void UInventoryComponent::Init(int32 InventorySlotNum)
{
	if(InventorySlotNum < 0)
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryComponent::Init\nInventorySlotNum < 0"));
		SetInventorySlots(0);
	}
	else
	{
		UE_LOG(LogInventory, Log, TEXT("InventoryComponent::Init\nInventorySlotNum: %d"), InventorySlotNum);
		SetInventorySlots(InventorySlotNum);
	}
}

// �����丵 �ʿ�
bool UInventoryComponent::AddItem(UItemDefinition* ItemDefinition, int32& ItemCount)
{
	UE_LOG(LogInventory, Log, TEXT("InventoryComponent::AddItem\nItemDefinition: %s / Count: %d"), *ItemDefinition->GetName(), ItemCount);

	// �Էµ� ������ ��ȿ�� �˻�
	if(ItemDefinition == nullptr || ItemCount <= 0)
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryComponent::AddItem\nItemCount <= 0"));
		return false;
	}

	const bool bAdded = FillSameItem(ItemDefinition, ItemCount);

	// ���ο� ������ Ȥ�� ���� ������ �뷮�� �ʰ��� ������ �߰�
	int32 Index;
	if(FindNewIndex(Index))
	{
		FInventoryItem NewInventoryItem(Index, 0, ItemDefinition);
		Inventory.Add(NewInventoryItem);
		return AddItem(ItemDefinition, ItemCount);
	}
	else
	{
		UE_LOG(LogInventory, Log, TEXT("bAdded: %s"), bAdded ? TEXT("true") : TEXT("false"));
		return bAdded;
	}
}

bool UInventoryComponent::FindNewIndex(int32& Index)
{
	// �κ��丮 ���� �� �� 
	if(Inventory.Num() == MaxInventorySlots)
	{
		return false;
	}

	// �������� ����ִ� �κ��丮 ���� �ε��� �˻�
	TArray<int32> Indices;
	for(FInventoryItem InventoryItem : Inventory)
	{
		Indices.Add(InventoryItem.InventoryIndex);
	}

	// ��� �ִ� �ε��� �� ���� ���� �ε��� ��ȯ
	for(int i = 0; i < MaxInventorySlots; i++)
	{
		if(Indices.Find(i) == INDEX_NONE)
		{
			Index = i;
			return true;
		}
	}

	//UE_LOG(LogInventory, Error, TEXT("InventoryComponent::FindNewIndex\n�κ��丮 ���� ��ȿ�� �˻� ����"));
	return false;
}

bool UInventoryComponent::FillSameItem(UItemDefinition* ItemDefinition, int32& ItemCount)
{
	// �κ��丮�� ������ ������ �˻� �� �뷮�� �� �� �����۵鸸 �ε��� ���
	TArray<int32> Indices;
	for(FInventoryItem InventoryItem : Inventory)
	{
		if(InventoryItem == ItemDefinition && InventoryItem.Addable())
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

