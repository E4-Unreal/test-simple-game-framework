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
// Equipment Sockets

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Sockets", ShortTooltip = "Skeletal Mesh Socket Names for Equipment Slots"))
class THIRDPERSON_API UEquipmentSlots : public UDataAsset
{
	GENERATED_BODY()

public:
	// Todo C++이 아니라 에디터에서 설정할 수 있는 게임플레이 태그를 제한하는 방법은 없을까?
	
	// Todo 프로젝트에 설정된 GameplayTag에 따라 meta=(Categories="") 커스터마이징 필요
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="EquipmentSlot"))
	TArray<FGameplayTag> EquipmentSlotTags;

	// Todo 프로젝트에 설정된 GameplayTag에 따라 meta=(Categories="") 커스터마이징 필요
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="EquipmentSlot.Weapon"))
	FGameplayTag PrimaryWeaponSlotTag;
};

//////////////////////////////////////////////////////////////////////
// Equipment Sockets

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Sockets", ShortTooltip = "Skeletal Mesh Socket Names for Equipment Slots"))
class THIRDPERSON_API UEquipmentSockets : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* SkeletalMeshAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FName> SocketMappings;
};

//////////////////////////////////////////////////////////////////////
// Equipment Component

class AEquipment;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(DisplayName = "Equipment Component", BlueprintSpawnableComponent))
class THIRDPERSON_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
	// 멤버 변수
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UEquipmentSlots* EquipmentSlots;

	// Todo EquipmentSockets->SocketMappings의 Key 값들을 EquipmentSlots에 들어있는 값으로만 설정 할 수 있는 방법은 없을까?
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UEquipmentSockets* EquipmentSockets;
	
protected:
	TArray<FEquipmentItem> EquipmentItems;

	// 멤버 함수
public:
	// Sets default values for this component's properties
	UEquipmentComponent();

	void Init();

	UFUNCTION(BlueprintCallable)
	bool AddEquipment(UEquipmentDefinition* NewEquipment);

protected:

	const FName* CheckSocket(FEquipmentItem EquipmentItem, const ACharacter* OwnerCharacter) const;
	AEquipment* SpawnEquipment(UEquipmentDefinition* NewEquipment) const;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

