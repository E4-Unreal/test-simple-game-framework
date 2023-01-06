// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ThirdPerson/ThirdPerson.h"
#include "ThirdPerson/Item/ItemDefinition.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UInventoryComponent::AddItem(class UItemDefinition* ItemDefinition, int32& ItemCount)
{
	// 구현 예정
	UE_LOG(LogInventory, Log, TEXT("AddItem"));
	UE_LOG(LogInventory, Log, TEXT("ItemDefinition: %s / Count: %d"), *ItemDefinition->GetName(), ItemCount);
	UE_LOG(LogInventory, Log, TEXT("ItemCount -= 30"));
	ItemCount -= 30;

	UE_LOG(LogInventory, Log, TEXT("ItemCount: %d"), ItemCount);
	return true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

