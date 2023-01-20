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

	// 멤버 변수
	class USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	// 스폰시 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	UItemDefinition* ItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	bool bDefaultCount = true;

	// 멤버 함수
protected:
	// 초기화
	// Init()에서 ItemDefinition 유효성 확인 끝남
	void Init();

	// 상호작용
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

};
