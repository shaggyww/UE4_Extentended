// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TeamAI_TestEditorTarget : TargetRules
{
	public TeamAI_TestEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        //ExtraModuleNames.Add("TeamAI_Test");
        ExtraModuleNames.AddRange(new string[] { "TeamAI_Test", "ExtNodeEditor" });
    }
}
