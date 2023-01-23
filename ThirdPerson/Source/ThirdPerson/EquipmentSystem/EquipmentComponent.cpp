// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki


#include "EquipmentComponent.h"
#include "Equipment.h"
#include "EquipmentState.h"
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
	// EquipmentSlotTags, EquipmentSockets 유효성 검사
	if(!IsValid(EquipmentSlotTags)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::Init > EquipmentSlotTags is not valid")) return; }
	if(!IsValid(EquipmentSockets)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::Init > EquipmentSockets is not valid")) return; }
	
	// 장비 슬롯(GameplayTag) 설정
	SelectableTag = FThirdPersonGameplayTags::Get().EquipmentSlot_Active;
	MainTag = FThirdPersonGameplayTags::Get().EquipmentSlot_Active_Main;
	PrimarySlot = EquipmentSlotTags->GetPrimary();

	// Todo 상태가 변할 때마다 상태 객체를 새로 생성하면 코드 관리가 편리하지만, 장비 상태가 수시로 변하기 때문에 상태별로 하나의 객체만 생성하여 관리중인데 둘 다 해결할 수 있는 방법이 없을까?
	// Equipment State 초기화
	MainState = NewObject<UMainState>(this, UMainState::StaticClass());
	MainState->CurrentSlot = PrimarySlot;
	SubState = NewObject<USubState>(this, USubState::StaticClass());
	EquipmentState = MainState;
	
	for(const FGameplayTag EquipmentSlot : EquipmentSlotTags->GetAll())
	{
		EquipmentItems.Add(FEquipmentItem(EquipmentSlot));
		UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::Init > EquipmentSlot Added: %s"), *EquipmentSlot.GetTagName().ToString())
	}
	
	UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::Init > EquipmentItems.Num(): %d"), EquipmentItems.Num())
}

bool UEquipmentComponent::AddEquipment(UEquipmentDefinition* NewEquipment)
{
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
			if(EquipmentItems.FindByKey(EquipmentState->CurrentSlot)->IsEmpty()){ SelectEquipment(EquipmentItem.EquipmentSlot); }

			// 이벤트 디스패처 호출
			OnUpdate.Broadcast();
			
			return true;
		}
	}
	UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > Equipment Slots are full or %s is not exist in Equipment Slots"), *NewEquipment->EquipmentSlotTag.GetTagName().ToString())
	return false;
}

bool UEquipmentComponent::SelectEquipment(const FGameplayTag SelectedSlot)
{
	if(!IsValid(EquipmentState)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SelectEquipment > EquipmentState is not valid")) return false;}
	
	// 이미 선택된 장비 슬롯이라면 무시
	if(SelectedSlot.MatchesTagExact(EquipmentState->CurrentSlot)){ UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SelectEquipment > Already Selected Slot")) return false; }
	
	// 선택한 장비 슬롯이 선택 가능한 슬롯인지 확인
	if(!SelectedSlot.MatchesTag(SelectableTag)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SelectEquipment > %s should be under %s"), *SelectedSlot.ToString(), *SelectableTag.ToString()) return false; }

	// 선택한 장비 슬롯이 존재하는지 확인
	if(!EquipmentSlotTags->GetAll().Contains(SelectedSlot)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SelectEquipment > %s is not exist in %s::All"), *SelectedSlot.ToString(), *EquipmentSlotTags->GetName()) return false; }

	// 무기 위치 스왑
	EquipmentState->SwapToSelectedEquipment(SelectedSlot);

	// Todo 선택된 장비와 플레이어의 입력 액션 바인딩
	// Binding Equipment with SelectedSlot....

	// 이벤트 디스패처 호출
	//OnEquipmentSwapped.Broadcast();
	
	return true;
	
}

bool UEquipmentComponent::SwapEquipmentsBySlot(FGameplayTag OriginSlot, FGameplayTag DestSlot)
{
	// 스왑 가능한 슬롯인지 확인
	UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SwapEquipmentsBySlot > %s to %s"),*OriginSlot.ToString(), *DestSlot.ToString())
	if(OriginSlot.RequestDirectParent() != DestSlot.RequestDirectParent()){ UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SwapEquipmentsBySlot > Not Same Category")) return false; }

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
	if(EquipmentState->CurrentSlot == OriginSlot)
	{
		SelectEquipment(DestSlot);
	}
	else if(EquipmentState->CurrentSlot == DestSlot)
	{
		SelectEquipment(OriginSlot);
	}
	else
	{
		MoveEquipmentToSlot(OriginSlot, OriginSlot);
		MoveEquipmentToSlot(DestSlot, DestSlot);
	}
	
	return true;
}

const FName* UEquipmentComponent::CheckSocket(const FGameplayTag EquipmentSlot) const
{
	// 유효성 검사
	if(!IsValid(EquipmentSockets)) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > EquipmentSockets not found")) return nullptr; }
	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!IsValid(OwnerCharacter)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::CheckSocket > OwnerCharacter not found")) return nullptr; }

	// EquipmentSlot에 대응하는 소켓 확인
	const FName* SocketName = EquipmentSockets->GetSocketMappings().Find(EquipmentSlot);
	if(SocketName == nullptr){ UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > Check %s is in %s / SocketName == nullptr"), *EquipmentSlot.GetTagName().ToString(), *EquipmentSockets->GetName()) return nullptr;}
	if(SocketName->IsNone()){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::CheckSocket > Check %s is initialized / SocketName->IsNone()"), *EquipmentSockets->GetName()) return nullptr;}

	// EquipmentSockets 데이터 에셋의 기준 스켈레탈 메시 에셋에 대응하는 소켓이 존재하는지 확인
	if(EquipmentSockets->GetSkeletalMeshAsset()->FindSocket(*SocketName) == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > Check %s is set in %s"), *SocketName->ToString(), *EquipmentSockets->GetSkeletalMeshAsset()->GetName())}
	
	// OwnerCharacter의 스켈레탈 메시 애셋에 대응하는 소켓이 존재하는지 확인
	if(OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->FindSocket(*SocketName) == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > Check %s is set in %s"), *SocketName->ToString(), *OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->GetName()) return nullptr; }

	return SocketName;
}

bool UEquipmentComponent::SpawnEquipment(const FGameplayTag EquipmentSlot)
{
	// 유효성 검사
	UEquipmentDefinition* EquipmentDefinition = GetEquipmentDefinition(EquipmentSlot);
	if(!IsValid(EquipmentDefinition)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SpawnEquipment > %s has no EquipmentDefinition"), *EquipmentSlot.ToString()) return false; }

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

void UEquipmentComponent::MoveEquipmentToSlot(const FGameplayTag OriginSlot, const FGameplayTag DestSlot)
{
	// 이동시킬 장비 확인
	AActor* Equipment = GetEquipment(OriginSlot);
	if(!IsValid(Equipment)){ UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::MoveEquipmentToSlot > No Equipment in %s"), *OriginSlot.ToString()) return; }

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

bool UEquipmentComponent::SetEquipmentDisabled(const bool bDisable, const FGameplayTag EquipmentSlot)
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

