// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wikis


#include "Equipment.h"
#include "ThirdPerson/ThirdPerson.h"
#include "EquipmentItem.h"

// Sets default values
AEquipment::AEquipment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 씬 컴포넌트 초기화
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

// EquipmentDefinition을 읽어들여 해당 장비로 변환
void AEquipment::ApplyEquipmentDefinition() const
{
	// 유효성 검사
	if(EquipmentDefinition == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("PickupItem::Init > EquipmentDefinition == nullptr")) return; }

	// Set Mesh & Destroy if it failed
	SkeletalMesh->SetSkeletalMesh(EquipmentDefinition->DisplayMesh);
}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 에디터에서 설정을 변경하는 경우
void AEquipment::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	ApplyEquipmentDefinition();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

// SpawnActor 혹은 에디터에서 생성하는 경우
void AEquipment::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ApplyEquipmentDefinition();
}
