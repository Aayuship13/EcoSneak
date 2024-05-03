// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EcoSneak : ModuleRules
{
	public EcoSneak(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
