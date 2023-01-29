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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickupItem | Display")
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickupItem | Display")
	class UStaticMeshComponent* Mesh;

	// 스폰시 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem | Item Information", meta = (ExposeOnSpawn="true"))
	UItemDefinition* ItemDefinition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem | Item Information", meta = (ExposeOnSpawn="true"))
	int32 ItemCount;
	
	// Todo VisibleAnywhere, BlueprintReadOnly 예정 - 스폰 시에만 조작할 수 있도록 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem | Item Information", meta = (ExposeOnSpawn="true"))
	bool bDefaultCount = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem | Display")
	float DefaultRadius = 100;

	// 멤버 함수
protected:
	
	// ItemDefinition을 읽어들여 해당 아이템으로 변환
	void ApplyItemDefinition();

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

	// 에디터에서 설정을 변경하는 경우
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// SpawnActor 혹은 에디터에서 생성하는 경우
	virtual void PostInitializeComponents() override;

};
