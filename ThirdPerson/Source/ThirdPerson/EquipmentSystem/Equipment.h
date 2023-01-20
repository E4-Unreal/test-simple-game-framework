// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

class UEquipmentDefinition;
UCLASS()
class THIRDPERSON_API AEquipment : public AActor
{
	GENERATED_BODY()

	// 멤버 변수
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	// 스폰시 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	UEquipmentDefinition* EquipmentDefinition;

	// 멤버 함수
public:
	// Sets default values for this actor's properties
	AEquipment();

	// 초기화
	void Init();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
