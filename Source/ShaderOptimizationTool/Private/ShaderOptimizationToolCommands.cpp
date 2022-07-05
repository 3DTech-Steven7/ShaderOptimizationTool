// Copyright 2022 Unireal, All rights reserved.

#include "ShaderOptimizationToolCommands.h"

#define LOCTEXT_NAMESPACE "FShaderOptimizationToolModule"

void FShaderOptimizationToolCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction_SkipShaderCompilation, "SkipShaderCompilation", "Execute SkipShaderCompilation action", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(PluginAction_ClearCache, "ClearCache", "Execute ClearCache action", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(PluginAction_CancelAllCompilations, "CancelAllCompilations", "Execute CancelAllCompilations action", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PluginAction_ReCompilations, "ReCompilations", "Execute ReCompilations action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
