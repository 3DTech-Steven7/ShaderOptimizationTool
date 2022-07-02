// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShaderOptimizationTool.h"
#include "ShaderOptimizationToolStyle.h"
#include "ShaderOptimizationToolCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "ShaderMangerHelper.h"

static const FName ShaderOptimizationToolTabName("ShaderOptimizationTool");

#define LOCTEXT_NAMESPACE "FShaderOptimizationToolModule"

void FShaderOptimizationToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FShaderOptimizationToolStyle::Initialize();
	FShaderOptimizationToolStyle::ReloadTextures();

	FShaderOptimizationToolCommands::Register();

	CommandListRef = MakeShareable(new FUICommandList);

	CommandListRef->MapAction(
		FShaderOptimizationToolCommands::Get().PluginAction_SkipShaderCompilation,
		FExecuteAction::CreateRaw(this, &FShaderOptimizationToolModule::OnToggleSkipShaderCompilation),
		FCanExecuteAction(),
		FIsActionChecked::CreateRaw(this, &FShaderOptimizationToolModule::IsShaderCompilationSkipped));

	CommandListRef->MapAction(
		FShaderOptimizationToolCommands::Get().PluginAction_ClearCache,
		FExecuteAction::CreateRaw(this, &FShaderOptimizationToolModule::ClearCache),
		FCanExecuteAction(),
		FIsActionChecked::CreateRaw(this, &FShaderOptimizationToolModule::IsClearCache));

	CommandListRef->MapAction(
		FShaderOptimizationToolCommands::Get().PluginAction_CancelAllCompilations,
		FExecuteAction::CreateRaw(this, &FShaderOptimizationToolModule::CancelAllCompilations),
		FCanExecuteAction::CreateRaw(this, &FShaderOptimizationToolModule::CheckCanCancel));

	CommandListRef->MapAction(
		FShaderOptimizationToolCommands::Get().PluginAction_ReCompilations,
		FExecuteAction::CreateRaw(this, &FShaderOptimizationToolModule::ReCompilations),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FShaderOptimizationToolModule::RegisterMenus));
}

void FShaderOptimizationToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FShaderOptimizationToolStyle::Shutdown();

	FShaderOptimizationToolCommands::Unregister();
}

void FShaderOptimizationToolModule::CancelAllCompilations()
{
	// Put your "OnButtonClicked" stuff here

	FShaderCompilingManagerHelper::CancelAllCompilations();
}

void FShaderOptimizationToolModule::ReCompilations()
{
	FShaderCompilingManagerHelper::ReCompilations();
}

void FShaderOptimizationToolModule::ClearCache()
{
	FShaderCompilingManagerHelper::ClearCache();
}

void FShaderOptimizationToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("ShaderOptimizationTool");
			{
				{
					FToolMenuEntry& Entry = Section.AddEntry(
						FToolMenuEntry::InitToolBarButton(
							FShaderOptimizationToolCommands::Get().PluginAction_SkipShaderCompilation,
							FText::FromString("SkipShaderCompilation"),
							LOCTEXT("SkipShaderCompilation", "注意，开启跳过材质编译功能后不要打开材质编辑器，已经打开也不要编译。"),
							FSlateIcon(TEXT("EditorStyle"), "MaterialEditor.Apply")
						));
					Entry.SetCommandList(CommandListRef);
				}

				{
					FToolMenuEntry& Entry = Section.AddEntry(
						FToolMenuEntry::InitToolBarButton(
							FShaderOptimizationToolCommands::Get().PluginAction_ClearCache,
							FText::FromString("ClearCache"),
							LOCTEXT("ClearCache", "开启将场景变为灰模效果，关闭则恢复默认显示效果，选择的物体不会变成灰模效果"),
							FSlateIcon(TEXT("EditorStyle"), "LandscapeEditor.RampTool")
						));
					Entry.SetCommandList(CommandListRef);
				}

				{
					FToolMenuEntry& Entry = Section.AddEntry(
						FToolMenuEntry::InitToolBarButton(
							FShaderOptimizationToolCommands::Get().PluginAction_CancelAllCompilations,
							FText::FromString("CancelAllCompilations"),
							LOCTEXT("CancelAllCompilations", "关闭当前材质编译的队列"),
							FSlateIcon(TEXT("EditorStyle"), "ContentReference.Clear")
						));
					Entry.SetCommandList(CommandListRef);
				}

				{
					FToolMenuEntry& Entry = Section.AddEntry(
						FToolMenuEntry::InitToolBarButton(
							FShaderOptimizationToolCommands::Get().PluginAction_ReCompilations,
							FText::FromString("ReCompilations"),
							LOCTEXT("ReCompilations", "可以选择或者将整个场景的材质重新编译"),
							FSlateIcon(TEXT("EditorStyle"), "LevelEditor.Recompile")
						));
					Entry.SetCommandList(CommandListRef);
				}
			}
		}
	}
}

bool FShaderOptimizationToolModule::CheckCanCancel() const
{
	return FShaderCompilingManagerHelper::CheckCanCancel();
}

bool FShaderOptimizationToolModule::IsShaderCompilationSkipped() const
{
	return FShaderCompilingManagerHelper::IsShaderCompilationSkipped();
}

bool FShaderOptimizationToolModule::IsClearCache() const
{
	return FShaderCompilingManagerHelper::IsClearCache();
}

void FShaderOptimizationToolModule::OnToggleSkipShaderCompilation()
{
	FShaderCompilingManagerHelper::OnToggleSkipShaderCompilation();
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FShaderOptimizationToolModule, ShaderOptimizationTool)