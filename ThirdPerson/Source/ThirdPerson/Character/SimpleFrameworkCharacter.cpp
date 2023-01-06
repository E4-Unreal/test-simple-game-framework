// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPerson/Character/SimpleFrameworkCharacter.h"
#include "EnhancedInputComponent.h"

ASimpleFrameworkCharacter::ASimpleFrameworkCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ASimpleFrameworkCharacter::Init()
{
	InteractionComponent->Init(FollowCamera);
	InventoryComponent->Init(InventorySlots);
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

void ASimpleFrameworkCharacter::BeginPlay()
{
	Super::BeginPlay();
	Init();
}
