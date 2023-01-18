// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Equipment.h"
#include "GameFramework/Character.h"
#include "ThirdPerson/GamePlayTags/ThirdPersonGameplayTags.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEquipmentComponent::Init()
{
	for(const FGameplayTag EquipmentSlot : EquipmentSlots)
	{
		EquipmentItems.Add(FEquipmentItem(EquipmentSlot));
	}
}

//ToDo 리팩토링이 필요해보임
bool UEquipmentComponent::AddEquipment(UEquipmentDefinition* NewEquipment)
{
	for(FEquipmentItem EquipmentItem : EquipmentItems)
	{
		if(EquipmentItem.IsAddable(NewEquipment))
		{
			// 유효성 검사
			if(NewEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::AddEquipment > NewEquipment == nullptr")) return false; }
			if(EquipmentSockets == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > EquipmentSockets == nullptr")) EquipmentItem.Add(NewEquipment, nullptr); return true; }
			
			// 해당 장비 슬롯에 대응하는 소켓 이름 확인
			const FName* SocketName = EquipmentSockets->SocketMappings.Find(EquipmentItem.EquipmentSlot);
			if(SocketName->IsNone()){ UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > SocketName->IsNone()")) EquipmentItem.Add(NewEquipment, nullptr); return true;}

			// Owner의 스켈레탈 메시 애셋에 대응하는 소켓이 존재하는지 확인
			const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
			USkeletalMeshSocket* SkeletalMeshSocket = OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->FindSocket(*SocketName);
			if(SkeletalMeshSocket == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > SkeletalMeshSocket == nullptr!")) EquipmentItem.Add(NewEquipment, nullptr); return true; }
			
			// 장비 스폰
			AEquipment* SpawnedEquipment = SpawnEquipment(NewEquipment);
			if(SpawnedEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::AddEquipment > SpawnedEquipment == nullptr!")) EquipmentItem.Add(NewEquipment, nullptr); return true; }

			// 장비를 소켓에 부착
			SpawnedEquipment->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
			EquipmentItem.Add(NewEquipment, SpawnedEquipment);
			return true;
		}
	}
	return false;
}

AEquipment* UEquipmentComponent::SpawnEquipment(UEquipmentDefinition* NewEquipment) const
{
	// 유효성 검사
	UWorld* World = GetWorld();
	if(World == nullptr) { return nullptr; }
	
	const FTransform SpawnLocAndRotation;

	AEquipment* SpawnedEquipment = World->SpawnActorDeferred<AEquipment>(NewEquipment->ClassToSpawn, SpawnLocAndRotation);
	if(SpawnedEquipment)
	{
		SpawnedEquipment->EquipmentDefinition = NewEquipment;
		SpawnedEquipment->FinishSpawning(SpawnLocAndRotation);
	}
	
	return SpawnedEquipment;
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	Init();
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

