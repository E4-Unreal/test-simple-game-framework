// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Interactable.h"
#include "Camera/CameraComponent.h"
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

	FCollisionQueryParams Params;
	// InteractionComponent가 부착된 액터 무시
	Params.AddIgnoredActor(GetOwner());
	
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		 PlayerCamera->GetComponentLocation(),
		PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * 1500.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5, 
		FCollisionShape::MakeSphere(10.0f),
		Params);
        
	if (bResult)
	{
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
			IInteractable::Execute_Interact(DetectedActor, GetOwner());
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

