// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPerson/Character/SimpleFrameworkCharacter.h"
#include "EnhancedInputComponent.h"

ASimpleFrameworkCharacter::ASimpleFrameworkCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->Init(FollowCamera);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ASimpleFrameworkCharacter::Interact()
{
	InteractionComponent->Interact();
}

void ASimpleFrameworkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Looking
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASimpleFrameworkCharacter::Interact);

	}
}
