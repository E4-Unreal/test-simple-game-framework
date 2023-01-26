// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentItem.h"

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

bool FEquipmentItem::Add(UEquipmentDefinition* NewEquipment)
{
	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::Add > EquipmentSlot: %s"), *EquipmentSlot.GetTagName().ToString())
	
	// 유효성 검사
	if(NewEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("FEquipmentItem::Add > NewEquipment == nullptr")) return false; }
	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::Add > EquipmentDefinition: %s"), *NewEquipment->GetName())
	
	EquipmentDefinition = NewEquipment;

	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::Add > Succeed"))
	return true;
}

bool FEquipmentItem::SetEquipment(AActor* SpawnedActor)
{
	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::SetEquipment > EquipmentSlot: %s"), *EquipmentSlot.GetTagName().ToString())
	
	// 유효성 검사
	if(SpawnedActor == nullptr) { UE_LOG(LogEquipment, Error, TEXT("FEquipmentItem::SetEquipment > SpawnedActor == nullptr")) return false; }

	UE_LOG(LogEquipment, Log, TEXT("FEquipmentItem::SetEquipment > SpawnedActor: %s"), *SpawnedActor->GetName())
	Equipment = SpawnedActor;
	return true;
}