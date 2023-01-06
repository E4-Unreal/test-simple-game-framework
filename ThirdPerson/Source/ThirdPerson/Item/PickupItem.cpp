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

	// ¾À ÄÄÆ÷³ÍÆ® ÃÊ±âÈ­
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = Sphere;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(Sphere);
}

void APickupItem::Init()
{
	if(ItemDefinition)
	{
		SetMesh();
		SetSphere();
		SetCount();
	}
}

void APickupItem::SetMesh()
{
	if(ItemDefinition->PickupInfo)
	{
		Mesh->SetStaticMesh(ItemDefinition->PickupInfo->DisplayMesh);
		Mesh->SetRelativeLocation(ItemDefinition->PickupInfo->MeshOffset);
	}
}

void APickupItem::SetSphere()
{
	if(ItemDefinition->PickupInfo)
	{
		Sphere->SetSphereRadius(ItemDefinition->PickupInfo->SphereRadius);
	}
}

void APickupItem::SetCount()
{
	if(ItemDefinition->PickupInfo)
	{
		ItemCount = ItemDefinition->SpawnCount;
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
	TArray<AActor*> OverlappingActors;
	Sphere->GetOverlappingActors(OverlappingActors);
	if(OverlappingActors.Find(Interactor))
	{
		AddItemToInventory(Interactor);
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

