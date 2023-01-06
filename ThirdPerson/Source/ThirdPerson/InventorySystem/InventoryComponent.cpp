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

// 리팩토링 필요
bool UInventoryComponent::AddItem(UItemDefinition* ItemDefinition, int32& ItemCount)
{
	UE_LOG(LogInventory, Log, TEXT("InventoryComponent::AddItem\nItemDefinition: %s / Count: %d"), *ItemDefinition->GetName(), ItemCount);

	// 입력된 아이템 유효성 검사
	if(ItemDefinition == nullptr || ItemCount <= 0)
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryComponent::AddItem\nItemCount <= 0"));
		return false;
	}

	const bool bAdded = FillSameItem(ItemDefinition, ItemCount);

	// 새로운 아이템 혹은 기존 아이템 용량을 초과한 아이템 추가
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
	// 인벤토리 슬롯 꽉 참 
	if(Inventory.Num() == MaxInventorySlots)
	{
		return false;
	}

	// 아이템이 들어있는 인벤토리 슬롯 인덱스 검색
	TArray<int32> Indices;
	for(FInventoryItem InventoryItem : Inventory)
	{
		Indices.Add(InventoryItem.InventoryIndex);
	}

	// 비어 있는 인덱스 중 가장 작은 인덱스 반환
	for(int i = 0; i < MaxInventorySlots; i++)
	{
		if(Indices.Find(i) == INDEX_NONE)
		{
			Index = i;
			return true;
		}
	}

	//UE_LOG(LogInventory, Error, TEXT("InventoryComponent::FindNewIndex\n인벤토리 슬롯 유효성 검사 실패"));
	return false;
}

bool UInventoryComponent::FillSameItem(UItemDefinition* ItemDefinition, int32& ItemCount)
{
	// 인벤토리에 동일한 아이템 검색 후 용량이 덜 찬 아이템들만 인덱스 기록
	TArray<int32> Indices;
	for(FInventoryItem InventoryItem : Inventory)
	{
		if(InventoryItem == ItemDefinition && InventoryItem.Addable())
		{
			Indices.Add(InventoryItem.InventoryIndex);
		}
	}

	// 한 번이라도 채웠다면 true 반환
	if(Indices.IsEmpty())
	{
		return false;
	}
	else
	{
		UE_LOG(LogInventory, Log, TEXT("InventoryComponent::FillSameItem"));
		Indices.Sort();

		// 차례대로 아이템 개수 추가
		for(const int32 Index : Indices)
		{
			// 입력 아이템 소진 : ItemCount <= 0
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

