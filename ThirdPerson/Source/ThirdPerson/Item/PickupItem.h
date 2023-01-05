// Fill out your copyright notice in the Description page of Project Settings.

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

	// 멤버 변수
	class USphereComponent* Sphere;
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemDefinition* ItemDefinition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemCount;

	// 멤버 함수
protected:
	// 초기화
	void Init();
	void SetMesh();
	void SetSphere();
	void SetCount();

	// 상호작용
	void AddItemToInventory(AActor* InventoryOwner);
	void Update(bool bSucceed, int32 Count);

	// IInteractable
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
