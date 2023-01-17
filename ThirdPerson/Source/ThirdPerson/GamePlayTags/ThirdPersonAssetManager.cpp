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

	//Load Native Tags
	FThirdPersonGameplayTags::InitializeNativeTags();
}