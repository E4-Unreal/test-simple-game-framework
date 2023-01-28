// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

//////////////////////////////////////////////////////////////////////
// Equipment
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
	class UEquipmentDefinition* EquipmentDefinition;

	// 멤버 함수
public:
	// Sets default values for this actor's properties
	AEquipment();

	// EquipmentDefinition을 읽어들여 스켈레탈 메시 설정
	void ApplyEquipmentDefinition() const;

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

//////////////////////////////////////////////////////////////////////
// Active Equipment
UCLASS()
class THIRDPERSON_API AActiveEquipment : public AEquipment
{
	GENERATED_BODY()
	
	// 멤버 변수
public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* MappingContext;

	// 멤버 함수
public:
	void Bind();
	void Unbind();
	
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
