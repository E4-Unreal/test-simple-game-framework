// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ThirdPerson/Item/ItemDefinition.h"
#include "EquipmentComponent.generated.h"

//////////////////////////////////////////////////////////////////////
// Equipment Slots

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Slot Tags", ShortTooltip = "Equipment SlotTags for Equipment Slots"))
class THIRDPERSON_API UEquipmentSlotTags : public UDataAsset
{
	GENERATED_BODY()

protected:
	// Todo C++이 아니라 에디터에서 설정할 수 있는 게임플레이 태그를 제한하는 방법은 없을까?
	
	// Todo 프로젝트에 설정된 GameplayTag에 따라 meta=(Categories="") 커스터마이징 필요
	// Todo FGameplayTagContainer로 변경?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot"))
	TArray<FGameplayTag> All;

	// Todo 프로젝트에 설정된 GameplayTag에 따라 meta=(Categories="") 커스터마이징 필요
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Active.Main"))
	FGameplayTag Primary;

public:
	FORCEINLINE TArray<FGameplayTag> GetAll() const { return All; }
	FORCEINLINE FGameplayTag GetPrimary() const { return Primary; }
};

//////////////////////////////////////////////////////////////////////
// Equipment Sockets

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Sockets", ShortTooltip = "Skeletal Mesh Socket Names for Equipment Slots"))
class THIRDPERSON_API UEquipmentSockets : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* SkeletalMeshAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="EquipmentSlot"))
	TMap<FGameplayTag, FName> SocketMappings;
	
public:
	FORCEINLINE USkeletalMesh* GetSkeletalMeshAsset() const { return SkeletalMeshAsset; }
	FORCEINLINE TMap<FGameplayTag, FName> GetSocketMappings() const { return SocketMappings; }
};

//////////////////////////////////////////////////////////////////////
// Equipment Component

class AEquipment;

// 이벤트 디스패처 매크로
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(DisplayName = "Equipment Component", BlueprintSpawnableComponent))
class THIRDPERSON_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
	// 멤버 변수
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UEquipmentSlotTags* EquipmentSlotTags;

	// Todo EquipmentSockets->SocketMappings의 Key 값들을 EquipmentSlots에 들어있는 값으로만 설정 할 수 있는 방법은 없을까?
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UEquipmentSockets* EquipmentSockets;
	
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
	FGameplayTag SelectableTag;
	FGameplayTag MainTag;
	FGameplayTag PrimarySlot;

	// 멤버 함수
public:
	// Sets default values for this component's properties
	UEquipmentComponent();

	void Init();

	UFUNCTION(BlueprintCallable)
	bool AddEquipment(UEquipmentDefinition* NewEquipment);

	UFUNCTION(BlueprintCallable)
	bool SelectEquipment(const FGameplayTag SelectedSlot);

	// For UI
	UFUNCTION(BlueprintPure, Category = "UI")
	TArray<FEquipmentItem> GetEquipmentItems() const { return EquipmentItems; }
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool RemoveEquipmentBySlot(FGameplayTag EquipmentSlot){ GetEquipment(EquipmentSlot)->Destroy(); GetEquipmentItem(EquipmentSlot)->Clear(); return true;}

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool SwapEquipmentsBySlot(FGameplayTag OriginSlot, FGameplayTag DestSlot);
	
	// 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FEquipmentDelegate OnUpdate;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FEquipmentDelegate OnEquipmentSwapped;

protected:
	// For EquipmentState
	FORCEINLINE void SetEquipmentState(UEquipmentState* NewState) { EquipmentState = NewState; }
	FORCEINLINE UMainState* GetMainState() const { return MainState; }
	FORCEINLINE USubState* GetSubState() const { return SubState; }

	// Function with EquipmentSlot
	const FName* CheckSocket(const FGameplayTag EquipmentSlot) const;
	bool SpawnEquipment(const FGameplayTag EquipmentSlot);
	FORCEINLINE FEquipmentItem* GetEquipmentItem(const FGameplayTag Slot){ return EquipmentItems.FindByKey(Slot); }
	FORCEINLINE UEquipmentDefinition* GetEquipmentDefinition(const FGameplayTag Slot){ if(GetEquipmentItem(Slot)){ return GetEquipmentItem(Slot)->EquipmentDefinition; } return nullptr; }
	FORCEINLINE AActor* GetEquipment(const FGameplayTag Slot) { if(GetEquipmentItem(Slot)){ return GetEquipmentItem(Slot)->Equipment; } return nullptr; }
	void MoveEquipmentToSlot(const FGameplayTag OriginSlot, const FGameplayTag DestSlot);
	bool SetEquipmentDisabled(const bool bDisable, const FGameplayTag EquipmentSlot);
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};