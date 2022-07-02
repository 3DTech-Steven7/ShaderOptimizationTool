// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ShaderOptimizationToolStyle.h"

class FShaderOptimizationToolCommands : public TCommands<FShaderOptimizationToolCommands>
{
public:

	FShaderOptimizationToolCommands()
		: TCommands<FShaderOptimizationToolCommands>(TEXT("ShaderOptimizationTool"), NSLOCTEXT("Contexts", "ShaderOptimizationTool", "ShaderOptimizationTool Plugin"), NAME_None, FShaderOptimizationToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction_SkipShaderCompilation;
	TSharedPtr< FUICommandInfo > PluginAction_ClearCache;
	TSharedPtr< FUICommandInfo > PluginAction_CancelAllCompilations;
	TSharedPtr< FUICommandInfo > PluginAction_ReCompilations;
};
