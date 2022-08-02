// Copyright 2022 3DTech, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FShaderOptimizationToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command. */
	void CancelAllCompilations();

	/** This function will be bound to Command. */
	void ReCompilations();
	void SwitchLembertView();
	bool CheckCanCancel();
	bool IsShaderCompilationSkipped();
	bool IsSwitchLembertView() const;
	bool CanSwitchLembertView() const;

	void OnToggleSkipShaderCompilation();

private:
	static void InitVariables();
	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> CommandListRef;
};
