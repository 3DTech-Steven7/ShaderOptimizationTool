// Copyright 2022 Unireal, All rights reserved.

using UnrealBuildTool;

public class ShaderOptimizationTool : ModuleRules
{
	public ShaderOptimizationTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"Engine",
				"Slate",
				"SlateCore",
				"RenderCore",
				"RHI",
				"Landscape"
			}
			);

	}
}
