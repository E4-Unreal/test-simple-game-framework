// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPerson/Character/BaseCharacter.h"
#include "ThirdPerson/InteractionSystem/InteractionComponent.h"
#include "ThirdPerson/InventorySystem/InventoryComponent.h"
#include "SimpleFrameworkCharacter.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_API ASimpleFrameworkCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASimpleFrameworkCharacter();

	UInteractionComponent* InteractionComponent;
	UInventoryComponent* InventoryComponent;
	
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryComponent")
	int32 InventorySlots = 0;

protected:
	void Init();
	/** Called for movement input */
	void Interact();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	virtual void BeginPlay() override;
};
