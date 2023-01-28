// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wikis


#include "Equipment.h"

#include "EnhancedInputSubsystems.h"
#include "ThirdPerson/ThirdPerson.h"
#include "EquipmentItem.h"

//////////////////////////////////////////////////////////////////////
// Equipment

// Sets default values
AEquipment::AEquipment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 씬 컴포넌트 초기화
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

// EquipmentDefinition을 읽어들여 해당 장비로 변환
void AEquipment::ApplyEquipmentDefinition() const
{
	// 유효성 검사
	if(EquipmentDefinition == nullptr) { UE_LOG(LogEquipment, Warning, TEXT("PickupItem::Init > EquipmentDefinition == nullptr")) return; }

	// Set Mesh & Destroy if it failed
	SkeletalMesh->SetSkeletalMesh(EquipmentDefinition->DisplayMesh);
}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 에디터에서 설정을 변경하는 경우
void AEquipment::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	ApplyEquipmentDefinition();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

// SpawnActor 혹은 에디터에서 생성하는 경우
void AEquipment::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ApplyEquipmentDefinition();
}

//////////////////////////////////////////////////////////////////////
// Active Equipment

void AActiveEquipment::Bind()
{
	// 유효성 검사
	if(!IsValid(GetOwner())){ UE_LOG(LogEquipment, Warning, TEXT("AActiveEquipment::Bind > %s: Owner is not valid"), *this->GetName()) return; }
	
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if(PlayerController == nullptr){ UE_LOG(LogEquipment, Warning, TEXT("AActiveEquipment::Bind > %s: Check Owner has player controller"), *this->GetName()) return; }

	// 플레이어 컨트롤러에 입력 액션 매핑 컨텍스트 추가
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		if(MappingContext == nullptr){ UE_LOG(LogEquipment, Warning, TEXT("AActiveEquipment::Bind > %s: MappingContext is nullptr"), *this->GetName()) return; }
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	// 플레이어 입력 활성화
	this->EnableInput(PlayerController);
}

void AActiveEquipment::Unbind()
{
	// 유효성 검사
	if(!IsValid(GetOwner())){ UE_LOG(LogEquipment, Warning, TEXT("AActiveEquipment::Bind > %s: Owner is not valid"), *this->GetName()) return; }
	
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if(PlayerController == nullptr){ UE_LOG(LogEquipment, Warning, TEXT("AActiveEquipment::Bind > %s: Check Owner has player controller"), *this->GetName()) return; }

	// 플레이어 컨트롤러에 입력 액션 매핑 컨텍스트 제거
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		if(MappingContext == nullptr){ UE_LOG(LogEquipment, Warning, TEXT("AActiveEquipment::Bind > %s: MappingContext is nullptr"), *this->GetName()) return; }
		Subsystem->RemoveMappingContext(MappingContext);
	}

	// 플레이어 입력 활성화
	this->EnableInput(PlayerController);
}

void AActiveEquipment::BeginPlay()
{
	Super::BeginPlay();
}

void AActiveEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActiveEquipment::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AActiveEquipment::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
