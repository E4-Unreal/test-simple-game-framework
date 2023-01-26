// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ThirdPerson/EquipmentSystem/Item/EquipmentItem.h"
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
	// Todo static으로 만들거나 이것도 설정 가능하도록 만들 예정
	FGameplayTag SelectableTag;
	FGameplayTag MainTag;
	FGameplayTag PrimarySlot;

	// 멤버 함수
public:
	// Sets default values for this component's properties
	UEquipmentComponent();

	void Init();

	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent")
	bool AddEquipment(UEquipmentDefinition* NewEquipment);

	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent")
	bool SelectEquipment(const FGameplayTag SelectedSlot);

	// For UI
	UFUNCTION(BlueprintPure, Category = "EquipmentComponent | UI")
	TArray<FEquipmentItem> GetEquipmentItems() const { return EquipmentItems; }
	
	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent | UI")
	bool RemoveEquipmentBySlot(FGameplayTag EquipmentSlot);

	UFUNCTION(BlueprintCallable, Category = "EquipmentComponent | UI")
	bool SwapEquipmentsBySlot(FGameplayTag OriginSlot, FGameplayTag DestSlot);
	
	// 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "EquipmentComponent | EvenetDispatcher")
	FEquipmentDelegate OnEquipmentAdded;
	
	UPROPERTY(BlueprintAssignable, Category = "EquipmentComponent | EvenetDispatcher")
	FEquipmentDelegate OnEquipmentSelected;
	
	UPROPERTY(BlueprintAssignable, Category = "EquipmentComponent | EvenetDispatcher")
	FEquipmentDelegate OnEquipmentSwapped;

protected:
	// For EquipmentState
	FORCEINLINE void SetEquipmentState(UEquipmentState* NewState) { EquipmentState = NewState; }
	FORCEINLINE UMainState* GetMainState() const { return MainState; }
	FORCEINLINE USubState* GetSubState() const { return SubState; }

	// Function with EquipmentSlot
	const FName* CheckSocket(const FGameplayTag EquipmentSlot) const;
	bool SpawnEquipment(const FGameplayTag EquipmentSlot);
	FORCEINLINE FEquipmentItem* GetEquipmentItem(const FGameplayTag Slot);
	FORCEINLINE UEquipmentDefinition* GetEquipmentDefinition(const FGameplayTag Slot);
	FORCEINLINE AActor* GetEquipment(const FGameplayTag Slot);
	void MoveEquipmentToSlot(const FGameplayTag OriginSlot, const FGameplayTag DestSlot);
	bool SetEquipmentDisabled(const bool bDisable, const FGameplayTag EquipmentSlot);
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// EquipmentComponent에서 사용하는 데이터 애셋들의 로딩이 완료된 후 초기화 진행
	virtual void PostLoad() override;
};