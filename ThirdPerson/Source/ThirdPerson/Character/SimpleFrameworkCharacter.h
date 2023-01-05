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
	UInteractionComponent* InteractionComponent;
	UInventoryComponent* InventoryComponent;
	
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

public:
	ASimpleFrameworkCharacter();

protected:
	/** Called for movement input */
	void Interact();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
};
