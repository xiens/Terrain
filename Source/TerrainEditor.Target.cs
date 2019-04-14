// Fill out Your copYright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TerrainEditorTarget : TargetRules
{
	public TerrainEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Terrain" } );
	}
}
