// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Base_MP_UE4GameMode.h"
#include "Base_MP_UE4Character.h"
#include "UObject/ConstructorHelpers.h"

ABase_MP_UE4GameMode::ABase_MP_UE4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
