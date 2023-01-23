// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThirdPerson/InteractionSystem/Interactable.h"
#include "PickupItem.generated.h"

class UItemDefinition;

UCLASS()
class THIRDPERSON_API APickupItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickupItem();

	// ��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickupItem | Display")
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickupItem | Display")
	class UStaticMeshComponent* Mesh;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem | Item Information", meta = (ExposeOnSpawn="true"))
	UItemDefinition* ItemDefinition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem | Item Information", meta = (ExposeOnSpawn="true"))
	int32 ItemCount;
	
	// Todo VisibleAnywhere, BlueprintReadOnly ���� - ���� �ÿ��� ������ �� �ֵ��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem | Item Information", meta = (ExposeOnSpawn="true"))
	bool bDefaultCount = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem | Display")
	float DefaultRadius = 100;

	// ��� �Լ�
protected:
	
	// ItemDefinition�� �о�鿩 �ش� ���������� ��ȯ
	void ApplyItemDefinition();

	// ��ȣ�ۿ�
	void AddItemToInventory(const AActor* InventoryOwner);
	void AddItemToEquipment(const AActor* EquipmentOwner);
	void Update();

	// IInteractable
	virtual void Interact_Implementation(const AActor* Interactor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �����Ϳ��� ������ �����ϴ� ���
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// SpawnActor Ȥ�� �����Ϳ��� �����ϴ� ���
	virtual void PostInitializeComponents() override;

};
