// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterInfo.generated.h"

/**
 * 
 */

class USkeletalMesh;

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Character Info", ShortTooltip = "Data asset used to configure a character."))
class THIRDPERSON_API UCharacterInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMesh* SkeletalMeshAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Transform|Mesh")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Transform|Mesh")
	FRotator Rotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation|Blueprint")
	TSubclassOf<UAnimInstance> AnimClass;

};