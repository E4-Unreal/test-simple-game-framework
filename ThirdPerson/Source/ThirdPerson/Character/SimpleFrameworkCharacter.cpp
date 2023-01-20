// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki


#include "ThirdPerson/Character/SimpleFrameworkCharacter.h"

#include "CharacterInfo.h"
#include "EnhancedInputComponent.h"

ASimpleFrameworkCharacter::ASimpleFrameworkCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ASimpleFrameworkCharacter::Init()
{
	InteractionComponent->Init(GetFollowCamera());
	InventoryComponent->Init(InventorySlots);
	ApplyCharacterInfo();
}

void ASimpleFrameworkCharacter::ApplyCharacterInfo()
{
	if(CharacterInfo)
	{
		GetMesh()->SetSkeletalMesh(CharacterInfo->SkeletalMeshAsset);
		GetMesh()->SetRelativeLocation(CharacterInfo->Location);
		GetMesh()->SetRelativeRotation(CharacterInfo->Rotator);
		GetMesh()->SetAnimInstanceClass(CharacterInfo->AnimClass);
	}
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
