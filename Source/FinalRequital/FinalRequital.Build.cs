// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FinalRequital : ModuleRules
{
	public FinalRequital(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
