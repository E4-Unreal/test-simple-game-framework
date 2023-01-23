// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wikis

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CharacterInfo | SkeletalMesh")
	USkeletalMesh* SkeletalMeshAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CharacterInfo | Transform | Mesh")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CharacterInfo | Transform | Mesh")
	FRotator Rotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CharacterInfo | Animation | Blueprint")
	TSubclassOf<UAnimInstance> AnimClass;

};