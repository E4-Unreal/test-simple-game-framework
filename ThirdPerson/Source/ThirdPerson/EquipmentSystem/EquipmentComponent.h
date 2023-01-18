// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ThirdPerson/Item/ItemDefinition.h"
#include "EquipmentComponent.generated.h"

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

UCLASS(ClassGroup=(Custom), meta=(DisplayName = "Equipment Component", BlueprintSpawnableComponent))
class THIRDPERSON_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
	// 멤버 변수
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> EquipmentSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
	
	AEquipment* SpawnEquipment(UEquipmentDefinition* NewEquipment) const;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

