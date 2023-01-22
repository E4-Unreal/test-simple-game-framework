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
	if(EquipmentSlotTags == nullptr){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::Init > EquipmentSlotTags is not found")) return; }
	if(EquipmentSockets == nullptr){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::Init > EquipmentSockets is not found")) return; }
	
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
			if(NewEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::AddEquipment > NewEquipment == nullptr")) return false; }
			
			// 장비 스폰
			AEquipment* SpawnedEquipment = SpawnEquipment(NewEquipment);
			if(SpawnedEquipment == nullptr) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::AddEquipment > SpawnedEquipment == nullptr")) EquipmentItem.Add(NewEquipment, nullptr); return true; }
			
			// EquipmentItem에 저장 후 장비를 해당 소켓에 부착
			EquipmentItem.Add(NewEquipment, SpawnedEquipment);
			MoveEquipmentToSlot(EquipmentItem.EquipmentSlot, EquipmentItem.EquipmentSlot);

			if(EquipmentItems.FindByKey(EquipmentState->CurrentSlot)->IsEmpty())
			{
				SelectEquipment(EquipmentItem.EquipmentSlot);
			}
			return true;
		}
	}
	UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::AddEquipment > Equipment Slots are full or %s is not exist in Equipment Slots"), *NewEquipment->EquipmentSlotTag.GetTagName().ToString())
	return false;
}

bool UEquipmentComponent::SelectEquipment(const FGameplayTag SelectedSlot)
{
	// 이미 선택된 장비 슬롯이라면 무시
	if(SelectedSlot.MatchesTagExact(EquipmentState->CurrentSlot)){ UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SelectEquipment > Already Selected Slot")) return false; }
	
	// 선택한 장비 슬롯이 선택 가능한 슬롯인지 확인
	if(!SelectedSlot.MatchesTag(SelectableTag)){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SelectEquipment > EquipmentSlot should be under %s"), *SelectableTag.ToString()) return false; }

	// 선택한 장비 슬롯이 존재하는지 확인
	if(!EquipmentSlotTags->GetAll().Contains(SelectedSlot)) { UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SelectEquipment > %s is not exist in EquipmentSlots->EquipmentSlotTags"), *SelectedSlot.ToString()) return false; }

	// EquipmentState에 따라 무기 스왑 처리
	EquipmentState->SwapToSelectedEquipment(SelectedSlot);
	return true;
	
}

const FName* UEquipmentComponent::CheckSocket(const FGameplayTag EquipmentSlot) const
{
	// 유효성 검사
	if(EquipmentSockets == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("EquipmentComponent::CheckSocket > EquipmentSockets not found")) return nullptr; }
	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(OwnerCharacter == nullptr){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::CheckSocket > OwnerCharacter not found")) return nullptr; }

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

void UEquipmentComponent::MoveEquipmentToSlot(const FGameplayTag OriginSlot, const FGameplayTag DestSlot)
{
	// 이동시킬 장비 확인
	AActor* Equipment = GetEquipment(OriginSlot);
	if(Equipment == nullptr){ UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::MoveEquipmentToSlot > No Equipment in %s"), *OriginSlot.ToString()) return; }

	// 이동할 소켓이 존재하고, 해당 소켓에 장비를 보관할 수 있다면 장비를 해당 소켓에 부착하고, 그렇지 않다면 장비 비활성화
	if(const FName* SocketName = CheckSocket(DestSlot))
	{
		// 장비가 비활성화된 상태라면 다시 활성화
		if(Equipment->IsHidden()){ SetActorDisabled(false, Equipment); }
		Equipment->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
		UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::MoveEquipmentToSlot > %s is attached to %s"), *Equipment->GetName(), *SocketName->ToString())
	}
	else
	{
		SetActorDisabled(true, Equipment);
	}
}

bool UEquipmentComponent::SetActorDisabled(const bool bDisable, AActor* SpawnedActor)
{
	// 유효성 검사
	if(SpawnedActor == nullptr){ UE_LOG(LogEquipment, Error, TEXT("EquipmentComponent::SetActorDisabled > SpawnedActor == nullptr")) return false; }

	// 액터 비활성화
	SpawnedActor->SetActorHiddenInGame(bDisable);
	SpawnedActor->SetActorEnableCollision(!bDisable);
	SpawnedActor->SetActorTickEnabled(!bDisable);

	UE_LOG(LogEquipment, Log, TEXT("EquipmentComponent::SetActorDisabled > %s is %s"), *SpawnedActor->GetName(), bDisable ? TEXT("disabled") : TEXT("enabled"))
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

