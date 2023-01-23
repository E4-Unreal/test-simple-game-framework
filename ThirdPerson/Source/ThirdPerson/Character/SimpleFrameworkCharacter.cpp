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

void ASimpleFrameworkCharacter::Init() const
{
	InteractionComponent->Init(GetFollowCamera());
	InventoryComponent->Init(InventorySlots);
}

// CharacterInfo�� �о�鿩 �ش� ĳ���ͷ� ��ȯ
void ASimpleFrameworkCharacter::ApplyCharacterInfo() const
{
	USkeletalMesh* SkeletalMesh = nullptr;
	TSubclassOf<UAnimInstance> AnimClass = nullptr;
	FTransform RelativeTransform = FTransform(FQuat(0,0,0,0), FVector(0,0,0), FVector(0,0,0));
	
	if(CharacterInfo)
	{
		SkeletalMesh = CharacterInfo->SkeletalMeshAsset;
		AnimClass = CharacterInfo->AnimClass;
		RelativeTransform = FTransform(CharacterInfo->Rotator.Quaternion(), CharacterInfo->Location, FVector(1,1,1));
	}
	
	GetMesh()->SetSkeletalMesh(SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(AnimClass);
	GetMesh()->SetRelativeTransform(RelativeTransform);
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

// �����Ϳ��� ������ �����ϴ� ���
void ASimpleFrameworkCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	ApplyCharacterInfo();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

// SpawnActor Ȥ�� �����Ϳ��� �����ϴ� ���
void ASimpleFrameworkCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ApplyCharacterInfo();
}
