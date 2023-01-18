// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "ThirdPerson/Item/ItemDefinition.h"
#include "ThirdPerson/ThirdPerson.h"


// Sets default values
AEquipment::AEquipment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 씬 컴포넌트 초기화
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
}

void AEquipment::Init()
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
	Init();
}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

