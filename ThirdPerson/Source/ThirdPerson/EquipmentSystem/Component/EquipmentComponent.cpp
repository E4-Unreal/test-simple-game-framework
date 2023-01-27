// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki


#include "EquipmentComponent.h"
#include "EquipmentSlots.h"
#include "EquipmentSockets.h"
#include "EquipmentState.h"
#include "GameFramework/Character.h"
#include "ThirdPerson/EquipmentSystem/Item/Equipment.h"
#include "ThirdPerson/EquipmentSystem/Item/EquipmentItem.h"

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
	// EquipmentSlotTags, EquipmentSockets 유효성 검사
	if(!IsValid(EquipmentSlots)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::Init > EquipmentSlotTags is not valid")) return; }
	if(!IsValid(EquipmentSockets)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::Init > EquipmentSockets is not valid")) return; }

	// EquipmentItems 생성 및 PrimarySlot 설정
	ApplyEquipmentSlots();

	// Todo 상태가 변할 때마다 상태 객체를 새로 생성하면 코드 관리가 편리하지만, 장비 상태가 수시로 변하기 때문에 상태별로 하나의 객체만 생성하여 관리중인데 둘 다 해결할 수 있는 방법이 없을까?
	// Equipment State 초기화
	MainState = NewObject<UMainState>(this, UMainState::StaticClass());
	SubState = NewObject<USubState>(this, USubState::StaticClass());
	EquipmentState = MainState;
	EquipmentState->SetCurrentSlot(PrimarySlot);
}

bool UEquipmentComponent::CheckEquipSlot(UEquipmentDefinition* NewEquipment)
{
	// 유효성 검사
	if(!IsValid(NewEquipment)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::ChekcEquipable > NewEquipment is not valid")) return false; }
	if(EquipmentItems.FindByKey(NewEquipment))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UEquipmentComponent::AddEquipment(UEquipmentDefinition* NewEquipment)
{
	// 장비 슬롯에 장착할 수 있는지 확인
	if(!CheckEquipSlot(NewEquipment)) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > %s is not equipable"), *NewEquipment->GetName()) return false; }
	for(FEquipmentItem& EquipmentItem : EquipmentItems)
	{
		if(EquipmentItem.IsAddable(NewEquipment))
		{
			// 유효성 검사
			if(!IsValid(NewEquipment)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::AddEquipment > NewEquipment is not valid")) return false; }

			// EquipmentItem에 EquipmentDefinition 저장
			EquipmentItem.Add(NewEquipment);
			
			// 장비 스폰 후 저장
			SpawnEquipment(EquipmentItem.EquipmentSlot);
			
			// 장비를 해당 소켓에 부착
			MoveEquipmentToSlot(EquipmentItem.EquipmentSlot, EquipmentItem.EquipmentSlot);

			// 현재 들고 있는 장비가 없다면, 습득한 장비를 선택
			if(EquipmentItems.FindByKey(EquipmentState->GetCurrentSlot())->IsEmpty()){ SelectEquipment(EquipmentItem.EquipmentSlot); }

			// 이벤트 디스패처 호출
			OnEquipmentAdded.Broadcast();
			
			return true;
		}
	}
	UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > Equipment Slots are full or %s is not exist in Equipment Slots"), *NewEquipment->EquipmentSlotTag.GetTagName().ToString())
	return false;
}

bool UEquipmentComponent::SelectEquipment(const FEquipmentSlot SelectedSlot)
{
	if(!IsValid(EquipmentState)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SelectEquipment > EquipmentState is not valid")) return false;}
	
	// 이미 선택된 장비 슬롯이라면 무시
	// Todo Refactoring
	if(SelectedSlot == (EquipmentState->GetCurrentSlot())){ UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SelectEquipment > Already Selected Slot")) return false; }

	// 선택할 수 있는 장비 슬롯인지 확인 (Active)
	
	
	// 선택한 장비 슬롯이 존재하는지 확인
	if(EquipmentItems.FindByKey(SelectedSlot)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SelectEquipment > %s is not exist in %s::All"), *SelectedSlot.GetName(), *EquipmentSlots->GetName()) return false; }

	// 무기 위치 스왑
	EquipmentState->SwapToSelectedEquipment(SelectedSlot);

	// Todo 선택된 장비와 플레이어의 입력 액션 바인딩
	// Binding Equipment with SelectedSlot....

	// 이벤트 디스패처 호출
	OnEquipmentSelected.Broadcast();
	
	return true;
	
}

bool UEquipmentComponent::RemoveEquipmentBySlot(FEquipmentSlot EquipmentSlot)
{
	GetEquipment(EquipmentSlot)->Destroy(); GetEquipmentItem(EquipmentSlot)->Clear(); return true;
}

bool UEquipmentComponent::SwapEquipmentsBySlot(const FEquipmentSlot OriginSlot, const FEquipmentSlot DestSlot)
{
	// 스왑 가능한 슬롯인지 확인
	UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SwapEquipmentsBySlot > %s to %s"),*OriginSlot.GetName(), *DestSlot.GetName())
	if(OriginSlot.Matches(DestSlot)){ UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SwapEquipmentsBySlot > Not Same Category")) return false; }

	// 장비 슬롯 스왑
	FEquipmentItem* OriginEquipmentItem = GetEquipmentItem(OriginSlot);
	FEquipmentItem* DestEquipmentItem = GetEquipmentItem(DestSlot);
	
	const FEquipmentItem Tem = *OriginEquipmentItem;
	OriginEquipmentItem->Clear();
	OriginEquipmentItem->Add(DestEquipmentItem->EquipmentDefinition);
	OriginEquipmentItem->SetEquipment(DestEquipmentItem->Equipment);

	DestEquipmentItem->Clear();
	DestEquipmentItem->Add(Tem.EquipmentDefinition);
	DestEquipmentItem->SetEquipment(Tem.Equipment);

	// 장비 부착 위치 업데이트
	// 현재 선택된 장비가 포함된 경우, CurrentSlot만 변경하여 현재 선택된 장비가 변경되지 않도록 한다
	if(EquipmentState->GetCurrentSlot() == OriginSlot)
	{
		EquipmentState->SetCurrentSlot(DestSlot);
	}
	else if(EquipmentState->GetCurrentSlot() == DestSlot)
	{
		EquipmentState->SetCurrentSlot(OriginSlot);
	}
	else
	{
		// 스왑된 장비 슬롯에 맞게 무기 부착 위치 업데이트
		MoveEquipmentToSlot(OriginSlot, OriginSlot);
		MoveEquipmentToSlot(DestSlot, DestSlot);

		// Todo Refactoring
		// 만약 현재 선택된 장비가 주 장비가 아니라면 PrimarySlot의 주 장비 비활성화
		if(!EquipmentState->GetCurrentSlot().IsMain(MainTag))
		{
			SetEquipmentDisabled(true, PrimarySlot);
		}
	}

	// 이벤트 디스패처 호출
	OnEquipmentSwapped.Broadcast();
	
	return true;
}

void UEquipmentComponent::ApplyEquipmentSlots()
{
	// Todo Init()에서 유효성 검사 중복
	if(!IsValid(EquipmentSlots)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::ApplyEquipmentSlots > EquipmentSlots is not valid")) return; }

	// EquipmentSlots를 바탕으로 EquipmentItems 생성
	const FMainEquipmentSlot MainEquipmentSlot = EquipmentSlots->GetMainEquipmentSlot();
	const TArray<FSubEquipmentSlot> SubEquipmentSlot = EquipmentSlots->GetSubEquipmentSlot();

	// Main EquipmentItems
	int32 Count = MainEquipmentSlot.SocketTags.Num();
	for(int i = 0; i < Count; i++)
	{
		EquipmentItems.Add(FEquipmentItem(FEquipmentSlot(MainEquipmentSlot, i)));
	}

	// Sub EquipmentItems
	for(const FSubEquipmentSlot& EquipmentSlot : SubEquipmentSlot)
	{
		Count = EquipmentSlot.SocketTags.Num();
		for(int i = 0; i < Count; i++)
		{
			EquipmentItems.Add(FEquipmentItem(FEquipmentSlot(EquipmentSlot, i)));
		}
	}

	for(FEquipmentItem EquipmentItem : EquipmentItems)
	{
		UE_LOG(LogEquipment, Log, TEXT("UEquipmentComponent::ApplyEquipmentSlots > %s"), *EquipmentItem.EquipmentSlot.GetName())
	}

	// PrimarySlot 설정
	MainTag = MainEquipmentSlot.SlotName;
	PrimarySlot = FEquipmentSlot(MainEquipmentSlot, 0);
}

const FName* UEquipmentComponent::CheckSocket(const FEquipmentSlot EquipmentSlot) const
{
	// 유효성 검사
	if(!IsValid(EquipmentSockets)) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > EquipmentSockets not found")) return nullptr; }
	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!IsValid(OwnerCharacter)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::CheckSocket > OwnerCharacter not found")) return nullptr; }

	// EquipmentSlot에 대응하는 소켓 확인
	const FName* SocketName = EquipmentSockets->GetRegisteredSockets().Find(EquipmentSlot.GetSocketTag());
	if(SocketName == nullptr){ UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > Check %s is in %s / SocketName == nullptr"), *EquipmentSlot.GetName(), *EquipmentSockets->GetName()) return nullptr;}
	if(SocketName->IsNone()){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::CheckSocket > Check %s is initialized / SocketName->IsNone()"), *EquipmentSockets->GetName()) return nullptr;}

	// EquipmentSockets 데이터 에셋의 기준 스켈레탈 메시 에셋에 대응하는 소켓이 존재하는지 확인
	if(EquipmentSockets->GetSkeletalMeshAsset()->FindSocket(*SocketName) == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > Check %s is set in %s"), *SocketName->ToString(), *EquipmentSockets->GetSkeletalMeshAsset()->GetName())}
	
	// OwnerCharacter의 스켈레탈 메시 애셋에 대응하는 소켓이 존재하는지 확인
	if(OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->FindSocket(*SocketName) == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > Check %s is set in %s"), *SocketName->ToString(), *OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->GetName()) return nullptr; }

	return SocketName;
}

bool UEquipmentComponent::SpawnEquipment(const FEquipmentSlot EquipmentSlot)
{
	// 유효성 검사
	UEquipmentDefinition* EquipmentDefinition = GetEquipmentDefinition(EquipmentSlot);
	if(!IsValid(EquipmentDefinition)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SpawnEquipment > %s has no EquipmentDefinition"), *EquipmentSlot.GetName()) return false; }

	// 장비 스폰
	UWorld* World = GetWorld();
	if(!IsValid(World)) { return false; }
	const FTransform SpawnLocAndRotation;
	if(AEquipment* SpawnedEquipment = World->SpawnActorDeferred<AEquipment>(EquipmentDefinition->ClassToSpawn, SpawnLocAndRotation))
	{
		SpawnedEquipment->EquipmentDefinition = EquipmentDefinition;
		SpawnedEquipment->FinishSpawning(SpawnLocAndRotation);

		// 스폰 된 장비 저장
		GetEquipmentItem(EquipmentSlot)->SetEquipment(SpawnedEquipment);
		return true;
	}
	
	return false;
}

// For EquipmentItems
FEquipmentItem* UEquipmentComponent::GetEquipmentItem(const FEquipmentSlot Slot)
{
	return EquipmentItems.FindByKey(Slot);
}

UEquipmentDefinition* UEquipmentComponent::GetEquipmentDefinition(const FEquipmentSlot Slot)
{
	if(GetEquipmentItem(Slot)){ return GetEquipmentItem(Slot)->EquipmentDefinition; } return nullptr;
}

AActor* UEquipmentComponent::GetEquipment(const FEquipmentSlot Slot)
{
	if(GetEquipmentItem(Slot)){ return GetEquipmentItem(Slot)->Equipment; } return nullptr;
}

void UEquipmentComponent::MoveEquipmentToSlot(const FEquipmentSlot OriginSlot, const FEquipmentSlot DestSlot)
{
	// 이동시킬 장비 확인
	AActor* Equipment = GetEquipment(OriginSlot);
	if(!IsValid(Equipment)){ UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::MoveEquipmentToSlot > No Equipment in %s"), *OriginSlot.GetName()) return; }

	// 이동할 소켓 이름 확인
	if(const FName* SocketName = CheckSocket(DestSlot))
	{
		// 장비가 비활성화된 상태라면 다시 활성화
		SetEquipmentDisabled(false, OriginSlot);
		
		// 장비를 소켓에 부착
		Equipment->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
		UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::MoveEquipmentToSlot > %s is attached to %s"), *Equipment->GetName(), *SocketName->ToString())
	}
	else
	{
		// 장비 비활성화
		SetEquipmentDisabled(true, OriginSlot);
	}
}

bool UEquipmentComponent::SetEquipmentDisabled(const bool bDisable, const FEquipmentSlot EquipmentSlot)
{
	// 유효성 검사
	AActor* SpawnedActor = GetEquipment(EquipmentSlot);
	if(!IsValid(SpawnedActor)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SetActorDisabled > SpawnedActor is not valid")) return false; }

	// 이미 비활성화된 장비를 비활성화 하지는 않는다
	if(SpawnedActor->IsHidden() && bDisable){ return false; }

	// 이미 활성화된 장비를 활성화 하지는 않는다
	if(!SpawnedActor->IsHidden() && !bDisable){ return false; }
	
	// 액터 비활성화
	SpawnedActor->SetActorHiddenInGame(bDisable);
	SpawnedActor->SetActorEnableCollision(!bDisable);
	SpawnedActor->SetActorTickEnabled(!bDisable);

	UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SetEquipmentDisabled > %s is %s"), *SpawnedActor->GetName(), bDisable ? TEXT("disabled") : TEXT("enabled"))
	return true;
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

