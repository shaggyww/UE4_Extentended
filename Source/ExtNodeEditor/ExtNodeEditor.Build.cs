// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ExtNodeEditor : ModuleRules
{
	public ExtNodeEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });

        PublicDependencyModuleNames.AddRange(new string[] {
            "TeamAI_Test" ,
            "UnrealEd",
            "BlueprintGraph",
            "GraphEditor",
            "PropertyEditor",
            "SlateCore",
            "Slate",
            "EditorStyle",
            "Kismet",
            "KismetCompiler"});

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "PropertyEditor" });
    }
}
