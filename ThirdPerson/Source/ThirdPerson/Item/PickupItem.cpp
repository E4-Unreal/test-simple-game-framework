// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#include "PickupItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ThirdPerson/ThirdPerson.h"
#include "ThirdPerson/GamePlayTags/ThirdPersonGameplayTags.h"
#include "ItemDefinition.h"
#include "ThirdPerson/InventorySystem/InventoryComponent.h"
#include "ThirdPerson/EquipmentSystem/EquipmentComponent.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �� ������Ʈ �ʱ�ȭ
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = Sphere;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(Sphere);
}

void APickupItem::Init()
{
	// ItemDefinition ��ȿ�� �˻� ���� �� ���� �ı�
	if(ItemDefinition == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PickupItem::Init > ItemDefinition is null. Execute self-destroy"))
		this->Destroy();
		return;
	}
	if(ItemDefinition->PickupInfo == nullptr){ UE_LOG(LogTemp, Warning, TEXT("PickupItem::Init > ItemDefinition->PickupInfo is null")) return; }

	// Set Mesh & Destroy if it failed
	if(!Mesh->SetStaticMesh(ItemDefinition->PickupInfo->DisplayMesh)){ this->Destroy(); return; }
	Mesh->SetRelativeLocation(ItemDefinition->PickupInfo->Location);
	Mesh->SetRelativeRotation(ItemDefinition->PickupInfo->Rotation);

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


void APickupItem::AddItemToInventory(const AActor* InventoryOwner)
{
	// ��ȿ�� �˻�
	if(InventoryOwner == nullptr){ UE_LOG(LogItem, Error, TEXT("PickupItem::AddItemToInventory > InventoryOwner == nullptr")); return; }
	
	// InventoryComponent ���� ���� Ȯ��
	UInventoryComponent* Inventory = InventoryOwner->FindComponentByClass<UInventoryComponent>();
	if(Inventory == nullptr){ UE_LOG(LogItem, Error, TEXT("PickupItem::AddItemToInventory > Check %s has InventoryComponent!"), *InventoryOwner->GetName()); return; }

	// InventoryComponent�� ������ �߰�
	if(Inventory->AddItem(ItemDefinition, ItemCount))
	{
		UE_LOG(LogItem, Log, TEXT("PickupItem::AddItemToInventory > %s picked up %s"), *InventoryOwner->GetName(), *ItemDefinition->GetName())
		Update();
	}
}

void APickupItem::AddItemToEquipment(const AActor* EquipmentOwner)
{
	// ��ȿ�� �˻�
	if(EquipmentOwner == nullptr){ UE_LOG(LogItem, Error, TEXT("PickupItem::AddItemToEquipment > EquipmentOwner == nullptr")) return; }
	
	// EquipmentComponent ���� ���� Ȯ��
	UEquipmentComponent* Equipment = EquipmentOwner->FindComponentByClass<UEquipmentComponent>();
	if(EquipmentOwner == nullptr){ UE_LOG(LogItem, Error, TEXT("PickupItem::AddItemToEquipment > Check %s has EquipmentOwnerComponent!"), *EquipmentOwner->GetName()) return; }

	// EquipmentComponent ������ �߰�
	UEquipmentDefinition* EquipmentDefinition = Cast<UEquipmentDefinition>(ItemDefinition);
	if(EquipmentDefinition == nullptr){ UE_LOG(LogItem, Error, TEXT("PickupItem::AddItemToEquipment > Check %s is EquipmentDefinition!"), *ItemDefinition->GetName()) return; }
	if(Equipment->AddEquipment(EquipmentDefinition))
	{
		UE_LOG(LogItem, Log, TEXT("PickupItem::AddItemToEquipment > %s picked up %s"), *EquipmentOwner->GetName(), *ItemDefinition->GetName())
		this->Destroy();
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

void APickupItem::Interact_Implementation(const AActor* Interactor)
{
	UE_LOG(LogInteraction, Log, TEXT("PickupItem::Interact > Activated"))
	
	// ��ȿ�� �˻�
	if(Interactor == nullptr){ UE_LOG(LogInteraction, Error, TEXT("PickupItem::Interact > Interactor == nullptr")) return; }
	if(!Sphere->IsOverlappingActor(Interactor)){ UE_LOG(LogInteraction, Warning, TEXT("PickupItem::Interact > %s is not Overlapping %s. Move closer to pick up item!"), *Interactor->GetName(), *this->GetName()) return; }

	// ������ �±� Ȯ��
	if(!ItemDefinition->ItemTag.MatchesTag(FThirdPersonGameplayTags::Get().Item)){ UE_LOG(LogInteraction, Error, TEXT("PickupItem::Interact > Check ItemTag is set in %s"), *ItemDefinition->GetName()) return; }
	if(ItemDefinition->ItemTag.MatchesTag(FThirdPersonGameplayTags::Get().Item_Equipment))
	{
		AddItemToEquipment(Interactor);
	}
	else
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

