// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThirdPerson/EquipmentSystem/Equipment/EquipmentItem.h"
#include "EquipmentComponent.generated.h"

class AEquipment;
class UEquipmentSlots;
class UEquipmentSockets;

// 이벤트 디스패처 매크로
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(DisplayName = "Equipment Component", BlueprintSpawnableComponent))
class THIRDPERSON_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
	// 멤버 변수
public:
	// Todo 플레이어의 스탯에 따라 사용할 수 있는 슬롯을 추가할 때는 const 제거할 수도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	const UEquipmentSlots* EquipmentSlots;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	const UEquipmentSockets* EquipmentSockets;
	
protected:
	TArray<FEquipmentItem> EquipmentItems;

	// For EquipmentState
	friend class UEquipmentState;
	friend class UMainState;
	friend class USubState;

	UPROPERTY()
	UEquipmentState* EquipmentState;
	UPROPERTY()
	UMainState* MainState;
	UPROPERTY()
	USubState* SubState;

	// For SelectEquipment()
	FEquipmentSlot PrimarySlot;

	// 멤버 함수
public:
	// Sets default values for this component's properties
	UEquipmentComponent();

	void Init();

	// For Equipment
	UFUNCTION(BlueprintPure, Category = "EquipmentComponent|Equipment")
	FORCEINLINE AEquipment* GetEquipment(FEquipmentSlot Slot);
	
	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent|Equipment")
	FORCEINLINE AEquipment* GetCurrentEquipment();

	// For EquipmentDefinition
	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent|EquipmentDefinition")
	bool CanEquip(UEquipmentDefinition* NewEquipment);
	
	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent|EquipmentDefinition")
	bool AddEquipment(UEquipmentDefinition* NewEquipment);

	UFUNCTION(BlueprintPure, Category = "EquipmentComponent|EquipmentDefinition")
	FORCEINLINE UEquipmentDefinition* GetEquipmentDefinition(FEquipmentSlot Slot);

	UFUNCTION(BlueprintPure, Category = "EquipmentComponent|EquipmentDefinition")
	FORCEINLINE UEquipmentDefinition* GetCurrentEquipmentDefinition(FEquipmentSlot Slot);

	// For Input
	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent|InputAction")
	bool SelectEquipment(FEquipmentSlot SelectedSlot);
	
	// For UI
	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent|UI")
	UEquipmentDefinition* RemoveEquipmentItem(FEquipmentSlot EquipmentSlot);

	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent|UI")
	bool SwapEquipmentItems(FEquipmentSlot OriginSlot, FEquipmentSlot DestSlot);
	
	// 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "EquipmentComponent|EvenetDispatchers")
	FEquipmentDelegate OnEquipmentUpdate;
	
	UPROPERTY(BlueprintAssignable, Category = "EquipmentComponent|EvenetDispatchers")
	FEquipmentDelegate OnEquipmentSelected;
	
	UPROPERTY(BlueprintAssignable, Category = "EquipmentComponent|EvenetDispatchers")
	FEquipmentDelegate OnEquipmentSwapped;

protected:
	// Depend on UEquipmentSlots
	void ApplyEquipmentSlots();
	
	// For EquipmentState
	FORCEINLINE void SetEquipmentState(UEquipmentState* NewState) { EquipmentState = NewState; }
	FORCEINLINE UMainState* GetMainState() const { return MainState; }
	FORCEINLINE USubState* GetSubState() const { return SubState; }

	// Function with FEquipmentSlot
	// Depend on UEquipmentSockets
	const FName* CheckSocket(const FEquipmentSlot EquipmentSlot) const;

	// Depend on UEquipmentDefinition
	bool SpawnEquipment(FEquipmentSlot EquipmentSlot);

	// Depend on FEquipmentItem
	FORCEINLINE FEquipmentItem* GetEquipmentItem(FEquipmentSlot& Slot);

	// Based on GetEquipmentItem(), GetEquipmentDefinition(), GetEquipment()
	void MoveEquipmentToSlot(const FEquipmentSlot OriginSlot, const FEquipmentSlot DestSlot);
	bool SetEquipmentDisabled(const bool bDisable, const FEquipmentSlot EquipmentSlot);
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};