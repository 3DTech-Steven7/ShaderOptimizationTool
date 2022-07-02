// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShaderOptimizationTool : ModuleRules
{
	public ShaderOptimizationTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"Runtime/Engine/Private", // for Engine/Private/Collision/PhysXCollision.h
				"Runtime/Landscape/Private",
				"../Shaders/Shared"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
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
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
