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

void UInventoryComponent::AddItem(class UItemDefinition* ItemDefinition, const int32 ItemCount, bool& bSucceed, int32& Remainder)
{
	// 구현 예정
	UE_LOG(LogInventory, Log, TEXT("AddItem"));
	UE_LOG(LogInventory, Log, TEXT("ItemDefinition: %s / Count: %d"), *ItemDefinition->GetName(), ItemCount);
	bSucceed = true;
	Remainder = 0;
	UE_LOG(LogInventory, Log, TEXT("bSucceed: %s / Remainder: %d"), bSucceed ? TEXT("true") : TEXT("false"), Remainder);
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

