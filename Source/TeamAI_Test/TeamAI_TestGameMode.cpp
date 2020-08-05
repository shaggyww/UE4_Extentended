// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TeamAI_TestGameMode.h"
#include "TeamAI_TestPlayerController.h"
#include "TeamAI_TestCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"
#include "TeamNameCustomization.h"

ATeamAI_TestGameMode::ATeamAI_TestGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATeamAI_TestPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//Custom properties
	PropertyModule.RegisterCustomPropertyTypeLayout("TeamName", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTeamNameCustomization::MakeInstance));
}