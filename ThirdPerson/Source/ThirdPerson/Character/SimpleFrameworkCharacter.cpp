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

// CharacterInfo를 읽어들여 해당 캐릭터로 변환
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

// 에디터에서 설정을 변경하는 경우
void ASimpleFrameworkCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	ApplyCharacterInfo();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

// SpawnActor 혹은 에디터에서 생성하는 경우
void ASimpleFrameworkCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ApplyCharacterInfo();
}
