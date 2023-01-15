// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefinition.h"
#include "ThirdPerson/ThirdPerson.h"

bool FInventoryItem::Add(int32& NewCount)
{
	const int32 AvailableCount = ItemDefinition->MaxStack - Count;
	
	if(NewCount >= AvailableCount)
	{
		Count += AvailableCount;
		NewCount -= AvailableCount;
		return true;
	}
	else if(NewCount > 0)
	{
		Count += NewCount;
		NewCount = 0;
		return true;
	}
	else if(NewCount == 0)
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryItem::Add\nNewCount: 0"));
		return false;
	}
	else
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryItem::Add\nNewCount < 0"));
		return false;
	}
}
