// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentMappingComponent.h"
#include "EquipmentComponent.h"


// Sets default values for this component's properties
UEquipmentMappingComponent::UEquipmentMappingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEquipmentMappingComponent::Bind()
{
	Unbind();
	AActiveEquipment* ActiveEquipment = Cast<AActiveEquipment>(EquipmentComponent->GetCurrentEquipment());
	if(!IsValid(ActiveEquipment)) { UE_LOG(LogEquipment, Log, TEXT("UEquipmentMappingComponent::Bind > ActiveEquipment is not valid")) return; }
	
	ActiveEquipment->Bind();
	CurrentEquipment = ActiveEquipment;
}

void UEquipmentMappingComponent::Unbind() const
{
	if(!IsValid(CurrentEquipment)) { UE_LOG(LogEquipment, Log, TEXT("UEquipmentMappingComponent::Bind > CurrentEquipment is not valid")) return; }
	CurrentEquipment->Unbind();
}

void UEquipmentMappingComponent::Init()
{
	EquipmentComponent = Cast<UEquipmentComponent>(GetOwner()->FindComponentByClass(UEquipmentComponent::StaticClass()));
	
	if(!IsValid(EquipmentComponent)){ UE_LOG(LogEquipment, Error, TEXT("UEquipmentMappingComponent::Init > Check %s has EquipmentComponent"), *GetOwner()->GetName()) }
	EquipmentComponent->OnEquipmentSelected.AddDynamic(this, &UEquipmentMappingComponent::Bind);
	
	UE_LOG(LogEquipment, Log, TEXT("UEquipmentMappingComponent::Init > %s / %s: Succeed"), *GetOwner()->GetName(), *this->GetName())
}


// Called when the game starts
void UEquipmentMappingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Init();
}


// Called every frame
void UEquipmentMappingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
