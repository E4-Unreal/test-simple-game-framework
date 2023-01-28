// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentMappingComponent.generated.h"


class UEquipmentComponent;
class AActiveEquipment;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(DisplayName = "Equipment Mapping Component", BlueprintSpawnableComponent))
class THIRDPERSON_API UEquipmentMappingComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	UEquipmentComponent* EquipmentComponent;
	UPROPERTY()
	AActiveEquipment* CurrentEquipment;
	
public:
	// Sets default values for this component's properties
	UEquipmentMappingComponent();

	void Init();
	
	UFUNCTION()
	void Bind();
	UFUNCTION()
	void Unbind() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
