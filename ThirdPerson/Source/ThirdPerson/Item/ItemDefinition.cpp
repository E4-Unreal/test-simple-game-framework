// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefinition.h"
#include "ThirdPerson/ThirdPerson.h"

//////////////////////////////////////////////////////////////////////
// Inventory Item

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
		UE_LOG(LogInventory, Warning, TEXT("InventoryItem::Add > NewCount: 0"));
		return false;
	}
	else
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryItem::Add > NewCount < 0"));
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
// Equipment Item

bool FEquipmentItem::IsAddable(const UEquipmentDefinition* NewEquipment) const
{
	// Check Empty
	if(!(EquipmentDefinition == nullptr && Equipment == nullptr)){ return false; }
	
	// Check NewEquipment can be added to here
	if(!EquipmentSlot.MatchesTag(NewEquipment->EquipmentSlotTag)){ return false; }

	return true;
}

bool FEquipmentItem::Add(UEquipmentDefinition* NewEquipment, AActor* SpawnedActor)
{
	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::Add > EquipmentSlot: %s"), *EquipmentSlot.GetTagName().ToString())
	// 유효성 검사
	if(NewEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("FEquipmentItem::Add > NewEquipment == nullptr")) return false; }
	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::Add > EquipmentDefinition: %s"), *NewEquipment->GetName())
	
	if(SpawnedActor == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("FEquipmentItem::Add > SpawnedActor == nullptr")) }
	else { UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::Add > SpawnedActors: %s"), *SpawnedActor->GetName()) }
	
	EquipmentDefinition = NewEquipment;
	Equipment = SpawnedActor;

	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::Add > Succeed"))
	return true;
}
