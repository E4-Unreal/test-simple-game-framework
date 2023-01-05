// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

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
	UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::AddItem %s %d"), *ItemDefinition->GetName(), ItemCount);
	bSucceed = true;
	Remainder = 0;
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

