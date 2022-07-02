// Copyright Epic Games, Inc. All Rights Reserved.

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
	void ClearCache();
	bool CheckCanCancel() const;
	bool IsShaderCompilationSkipped() const;
	bool IsClearCache() const;

	void OnToggleSkipShaderCompilation();

private:

	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> CommandListRef;
};
