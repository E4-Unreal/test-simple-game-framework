// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
protected:
	FEquipmentSlot CurrentSlot;
	FEquipmentSlot SelectedSlot;

	// Get From EquipmentComponent in Constructor
	UPROPERTY()
	UEquipmentComponent* EquipmentComponent;

public:
	UEquipmentState();
	
	virtual void SwapToMainEquipment() {}
	virtual void SwapToSubEquipment() {}
	virtual void SwapToSelectedEquipment(FEquipmentSlot Slot);
	FORCEINLINE FEquipmentSlot GetCurrentSlot() const { return CurrentSlot; }
	FORCEINLINE void SetCurrentSlot(FEquipmentSlot Slot);

protected:
	// EquipmentComponent 멤버 변수 사용 간소화
	FORCEINLINE FEquipmentSlot GetPrimarySlot() const { return EquipmentComponent->PrimarySlot; }
	// Todo EquipmentState에서 초기화 하는 방법?
	FORCEINLINE UMainState* GetMainState() const { return EquipmentComponent->GetMainState(); }
	FORCEINLINE USubState* GetSubState() const { return EquipmentComponent->GetSubState(); }

	// EquipmentComponent 멤버 함수 호출 간소화
	FORCEINLINE void ChangeState(UEquipmentState* NewState) const;
	FORCEINLINE void RestoreEquipmentToSlot(const FEquipmentSlot EquipmentSlot) const { EquipmentComponent->MoveEquipmentToSlot(EquipmentSlot, EquipmentSlot); }
	FORCEINLINE void SwapSlots(const FEquipmentSlot OriginSlot, const FEquipmentSlot DestSlot) const
	{
		EquipmentComponent->MoveEquipmentToSlot(OriginSlot, DestSlot);
		EquipmentComponent->MoveEquipmentToSlot(DestSlot, OriginSlot);
	}
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
	virtual void SwapToSelectedEquipment(const FEquipmentSlot Slot) override;

protected:
	// 투척물과 같은 서브 장비들은 사용 후 원래 들고 있던 주 장비로 자동 스왑이 필요
	void ReturnToMainState() const;
	
};