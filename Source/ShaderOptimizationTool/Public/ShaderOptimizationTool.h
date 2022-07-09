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
	bool CheckCanCancel() const;
	bool IsShaderCompilationSkipped() const;
	bool IsSwitchLembertView() const;
	bool CanSwitchLembertView() const;

	void OnToggleSkipShaderCompilation();

private:

	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> CommandListRef;
};
