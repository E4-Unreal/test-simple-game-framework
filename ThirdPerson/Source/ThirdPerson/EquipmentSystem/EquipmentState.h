// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "EquipmentComponent.h"
#include "EquipmentState.generated.h"

/**
 * 
 */

//////////////////////////////////////////////////////////////////////
// Equipment State

class UEquipmentComponent;

UCLASS(Abstract)
class THIRDPERSON_API UEquipmentState : public UObject
{
	GENERATED_BODY()
public:
	FGameplayTag CurrentSlot;
	
protected:
	UEquipmentComponent* EquipmentComponent;
	FGameplayTag SelectedSlot;

public:
	UEquipmentState();
	
	virtual void SwapToMainEquipment() {}
	virtual void SwapToSubEquipment() {}
	virtual void SwapToSelectedEquipment(FGameplayTag Slot);

protected:
	FORCEINLINE void ChangeState(UEquipmentState* NewState) const { EquipmentComponent->SetEquipmentState(NewState); UE_LOG(LogEquipment, Log, TEXT("EquipmentState::ChangeState > EquipmentState Changed to %s"), *NewState->GetName()) }
	FORCEINLINE void RestoreEquipmentToSlot(const FGameplayTag EquipmentSlot) const { EquipmentComponent->MoveEquipmentToSlot(EquipmentSlot, EquipmentSlot); }
	FORCEINLINE void SwapSlots(const FGameplayTag OriginSlot, const FGameplayTag DestSlot) const
	{
		EquipmentComponent->MoveEquipmentToSlot(OriginSlot, DestSlot);
		EquipmentComponent->MoveEquipmentToSlot(DestSlot, OriginSlot);
	}
	FORCEINLINE void SetEquipmentDisabled(bool bDisable, FGameplayTag EquipmentSlot) const { if(AActor* Equipment = EquipmentComponent->GetEquipment(EquipmentSlot)){ EquipmentComponent->SetActorDisabled(bDisable, Equipment); } }
};

//////////////////////////////////////////////////////////////////////
// Main Equipment

UCLASS()
class THIRDPERSON_API UMainState : public UEquipmentState
{
	GENERATED_BODY()
	
public:
	virtual void SwapToMainEquipment() override;
	virtual void SwapToSubEquipment() override;
};

//////////////////////////////////////////////////////////////////////
// Sub Equipment

UCLASS()
class THIRDPERSON_API USubState : public UEquipmentState
{
	GENERATED_BODY()

public:
	virtual void SwapToMainEquipment() override;
	virtual void SwapToSubEquipment() override;
	virtual void SwapToSelectedEquipment(FGameplayTag Slot) override;

protected:
	// 투척물과 같은 서브 장비들은 사용 후 원래 들고 있던 주 장비로 자동 스왑이 필요
	void ReturnToMainState();
	
};