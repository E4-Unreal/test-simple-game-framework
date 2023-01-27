// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#include "ThirdPersonAssetManager.h"
#include "ThirdPersonGameplayTags.h"

UThirdPersonAssetManager::UThirdPersonAssetManager()
{

}

UThirdPersonAssetManager& UThirdPersonAssetManager::Get()
{
	check(GEngine);
	
	UThirdPersonAssetManager* ThirdPersonAssetManager = Cast<UThirdPersonAssetManager>(GEngine->AssetManager);

	if(ThirdPersonAssetManager)
	{
		return *ThirdPersonAssetManager;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, Must set this to ThirdPersonAssetManager"));
		return *NewObject<UThirdPersonAssetManager>();
	}
}

void UThirdPersonAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//Load Custom Native Tags
	FThirdPersonGameplayTags::InitializeNativeTags();
}