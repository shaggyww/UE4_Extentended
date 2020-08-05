// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExtNodeEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

#include "TeamNamePinFactory.h"

IMPLEMENT_GAME_MODULE(FExtNodeEditorModule, ExtNodeEditor);

#define LOCTEXT_NAMESPACE "ExtNodeEditor"

void FExtNodeEditorModule::StartupModule()
{
	//create your factory and shared pointer to it.
	TSharedPtr<FTeamNamePinFactory> teamNamePinFactory = MakeShareable(new FTeamNamePinFactory());
	//and now register it.
	FEdGraphUtilities::RegisterVisualPinFactory(teamNamePinFactory);

    //UE_LOG(ExtNodeEditor, Warning, TEXT("ExtNodeEditor: Log Started"));
}

void FExtNodeEditorModule::ShutdownModule()
{
    //UE_LOG(ExtNodeEditor, Warning, TEXT("ExtNodeEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE