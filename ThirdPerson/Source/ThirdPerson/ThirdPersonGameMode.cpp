// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonGameMode.h"
#include "ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonGameMode::AThirdPersonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/CPP/BP_ThirdPersonCharacterCPP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
