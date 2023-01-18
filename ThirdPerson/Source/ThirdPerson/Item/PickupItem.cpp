// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"

#include "ItemDefinition.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ThirdPerson/InventorySystem/InventoryComponent.h"
#include "ThirdPerson/ThirdPerson.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 씬 컴포넌트 초기화
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = Sphere;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(Sphere);
}

void APickupItem::Init()
{
	// ItemDefinition 유효성 검사
	if(ItemDefinition == nullptr){ UE_LOG(LogTemp, Warning, TEXT("PickupItem::Init\nItemDefinition is null")) return; }
	if(ItemDefinition->PickupInfo == nullptr){ UE_LOG(LogTemp, Warning, TEXT("PickupItem::Init\nItemDefinition->PickupInfo is null")) return; }

	// Set Mesh & Destroy if it failed
	if(!Mesh->SetStaticMesh(ItemDefinition->PickupInfo->DisplayMesh)){ this->Destroy(); return; }
	Mesh->SetRelativeLocation(ItemDefinition->PickupInfo->MeshOffset);

	// Set Sphere & Do auto scaling if default is 0 or less
	const float AutoScaling = Mesh->GetStaticMesh()->GetBounds().GetBox().GetSize().Size();
	const float SphereRadius = ItemDefinition->PickupInfo->SphereRadius;
	if(SphereRadius < AutoScaling)
	{
		Sphere->SetSphereRadius(AutoScaling);
		UE_LOG(LogTemp, Warning, TEXT("PickupItem::Init\nAuto scaling SphereRadius: %f"), AutoScaling);
	}
	else
	{
		Sphere->SetSphereRadius(SphereRadius);
	}

	// Set Count
	if(bDefaultCount)
	{
		ItemCount = ItemDefinition->SpawnCount;
	}
	else
	{
		if(ItemCount <= 0)
		{
			this->Destroy();
		}
	}

}


void APickupItem::AddItemToInventory(AActor* InventoryOwner)
{
	UInventoryComponent* Inventory = InventoryOwner->FindComponentByClass<UInventoryComponent>();
	if(Inventory)
	{
		if(Inventory->AddItem(ItemDefinition, ItemCount))
		{
			Update();
		}
	}
}

void APickupItem::Update()
{
	if(ItemCount == 0)
	{
		this->Destroy();
	}
	else if(ItemCount < 0)
	{
		UE_LOG(LogInventory, Error, TEXT("PickupItem :: ItemCount < 0"));
		this->Destroy();
	}
}

void APickupItem::Interact_Implementation(AActor* Interactor)
{
	UE_LOG(LogInteraction, Log, TEXT("PickupItem::Interact\nActivated"));
	if(Sphere->IsOverlappingActor(Interactor))
	{
		UE_LOG(LogInteraction, Log, TEXT("PickupItem::Interact\nTry adding item to inventory"));
		AddItemToInventory(Interactor);
	}
	else
	{
		UE_LOG(LogInteraction, Warning, TEXT("PickupItem::Interact\nMove closer to pick up item"));
	}
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

