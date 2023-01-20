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
	for(const FGameplayTag EquipmentSlot : EquipmentSlots->EquipmentSlotTags)
	{
		EquipmentItems.Add(FEquipmentItem(EquipmentSlot));
	}
}

bool UEquipmentComponent::AddEquipment(UEquipmentDefinition* NewEquipment)
{
	for(FEquipmentItem EquipmentItem : EquipmentItems)
	{
		if(EquipmentItem.IsAddable(NewEquipment))
		{
			// 유효성 검사
			if(NewEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::AddEquipment > NewEquipment == nullptr")) return false; }

			// 장비 슬롯에 대응하는 소켓명 확인
			const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
			if(OwnerCharacter == nullptr){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::AddEquipment > OwnerCharacter == nullptr")) return nullptr; }
			const FName* SocketName = CheckSocket(EquipmentItem, OwnerCharacter);
			if(SocketName == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > SocketName == nullptr")) EquipmentItem.Add(NewEquipment, nullptr); return true; }
			
			// 장비 스폰
			AEquipment* SpawnedEquipment = SpawnEquipment(NewEquipment);
			if(SpawnedEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::AddEquipment > SpawnedEquipment == nullptr")) EquipmentItem.Add(NewEquipment, nullptr); return true; }
			
			// 장비를 소켓에 부착
			SpawnedEquipment->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
			EquipmentItem.Add(NewEquipment, SpawnedEquipment);
			return true;
		}
	}
	UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > Equipment Slots are full or %s is not exist in Equipment Slots"), *NewEquipment->EquipmentSlotTag.GetTagName().ToString())
	return false;
}

const FName* UEquipmentComponent::CheckSocket(FEquipmentItem EquipmentItem, const ACharacter* OwnerCharacter) const
{
	// 유효성 검사
	if(EquipmentSockets == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > EquipmentSockets == nullptr")) return nullptr; }
	if(OwnerCharacter == nullptr){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::CheckSocket > OwnerCharacter == nullptr")) return nullptr; }

	// 해당 장비 슬롯에 대응하는 소켓 이름 확인
	const FName* SocketName = EquipmentSockets->SocketMappings.Find(EquipmentItem.EquipmentSlot);
	if(SocketName == nullptr){ UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > Check %s is in %s / SocketName == nullptr"), *EquipmentItem.EquipmentSlot.GetTagName().ToString(), *EquipmentSockets->GetName()) return nullptr;}
	if(SocketName->IsNone()){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::CheckSocket > Check %s is initialized / SocketName->IsNone()"), *EquipmentSockets->GetName()) return nullptr;}

	// EquipmentSockets 데이터 에셋의 기준 스켈레탈 메시 에셋에 대응하는 소켓이 존재하는지 확인
	if(EquipmentSockets->SkeletalMeshAsset->FindSocket(*SocketName) == nullptr) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::CheckSocket > Check %s is set in %s"), *SocketName->ToString(), *EquipmentSockets->SkeletalMeshAsset->GetName())}
	
	// OwnerCharacter의 스켈레탈 메시 애셋에 대응하는 소켓이 존재하는지 확인
	if(OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->FindSocket(*SocketName) == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > Check %s is set in %s"), *SocketName->ToString(), *OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->GetName()) return nullptr; }

	return SocketName;
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

	// 스폰된 장비의 스켈레탈 메시가 제대로 설정되었는지 확인
	if(SpawnedEquipment->SkeletalMesh->GetSkeletalMeshAsset() == nullptr)
	{
		UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SpawnEquipment > SpawnedEquipment->SkeletalMesh->GetSkeletalMeshAsset() == nullptr"))
		SpawnedEquipment->Destroy();
		return nullptr;
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

