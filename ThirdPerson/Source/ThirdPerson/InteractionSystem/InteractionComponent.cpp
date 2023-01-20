// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Interactable.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ThirdPerson/ThirdPerson.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInteractionComponent::Init(UCameraComponent* Camera)
{
	BindCamera(Camera);
}

void UInteractionComponent::BindCamera(UCameraComponent* Camera)
{
	PlayerCamera = Camera;
}

AActor* UInteractionComponent::DetectActor()
{
	if(!PlayerCamera)
	{
		UE_LOG(LogInteraction, Error, TEXT("InteractionComponent::DetectActor\nPlayerCamera is nullptr"));
		return nullptr;
	}
	
	FHitResult HitResult;
	
	// SphereTrace에서 무시할 액터들
	TArray<AActor*> ActorsToIgnore;

	// todo EDrawDebugTrace를 에디터 노출시키고 싶다
	bool bResult = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetOwner()->GetActorLocation(),
		PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * 1500.0f,
		10.0f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true);
        
	if (bResult)
	{
		UE_LOG(LogInteraction, Log, TEXT("InteractionComponent::DetectActor > %s"), *HitResult.GetActor()->GetName());
		return HitResult.GetActor();
	}
	else
	{
		return nullptr;
	}
}

void UInteractionComponent::Interact()
{
	if(AActor* DetectedActor = DetectActor())
	{
		// UInteractable 상속 여부 체크
		if(DetectedActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			UE_LOG(LogInteraction, Log, TEXT("InteractionComponent::Interact > %s can use IInteractable::Execute_Interact"), *DetectedActor->GetName());
			IInteractable::Execute_Interact(DetectedActor, GetOwner());
		}
		else
		{
			UE_LOG(LogInteraction, Warning, TEXT("InteractionComponent::Interact > %s cannot use IInteractable::Execute_Interact"), *DetectedActor->GetName());
		}
	}
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

