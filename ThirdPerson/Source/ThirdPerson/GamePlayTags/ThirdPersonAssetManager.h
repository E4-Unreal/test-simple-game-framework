#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ThirdPersonAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_API UThirdPersonAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	UThirdPersonAssetManager();

	// Returns the AssetManager singleton object.
	static UThirdPersonAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};