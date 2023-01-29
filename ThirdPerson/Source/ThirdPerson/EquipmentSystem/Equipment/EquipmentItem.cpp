// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentItem.h"
#include "Equipment.h"

//////////////////////////////////////////////////////////////////////
// Equipment Item

bool FEquipmentItem::IsAddable(const UEquipmentDefinition* NewEquipment) const
{
	// Check Empty
	if(!IsEmpty()){ return false; }
	// Check NewEquipment can be added to here
	if(!EquipmentSlot.MatchesTag(NewEquipment->EquipmentSlotTag)){ return false; }

	return true;
}

bool FEquipmentItem::SetEquipmentDefinition(UEquipmentDefinition* NewEquipment)
{
	// 유효성 검사
	if(NewEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("FEquipmentItem::Add > NewEquipment == nullptr")) return false; }
	EquipmentDefinition = NewEquipment;
	
	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::SetEquipmentDefinition > EquipmentSlot: %s / EquipmentDefinition: %s"), *EquipmentSlot.GetName(), *NewEquipment->GetName())
	return true;
}

bool FEquipmentItem::SetEquipment(AEquipment* SpawnedActor)
{
	// 유효성 검사
	if(SpawnedActor == nullptr) { UE_LOG(LogEquipment, Error, TEXT("FEquipmentItem::SetEquipment > SpawnedActor == nullptr")) return false; }
	Equipment = SpawnedActor;

	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::SetEquipment > EquipmentSlot: %s / SpawnedActor: %s"), *EquipmentSlot.GetName(), *SpawnedActor->GetName())
	return true;
}