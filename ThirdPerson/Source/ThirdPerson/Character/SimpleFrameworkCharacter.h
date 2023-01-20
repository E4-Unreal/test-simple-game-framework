// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "ThirdPerson/Character/BaseCharacter.h"
#include "ThirdPerson/InteractionSystem/InteractionComponent.h"
#include "ThirdPerson/InventorySystem/InventoryComponent.h"
#include "SimpleFrameworkCharacter.generated.h"

/**
 * 
 */
class UCharacterInfo;

UCLASS()
class THIRDPERSON_API ASimpleFrameworkCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASimpleFrameworkCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterInfo* CharacterInfo;

	UInteractionComponent* InteractionComponent;
	UInventoryComponent* InventoryComponent;
	
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryComponent")
	int32 InventorySlots = 0;

protected:
	void Init();
	void ApplyCharacterInfo();
	/** Called for movement input */
	void Interact();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
};
