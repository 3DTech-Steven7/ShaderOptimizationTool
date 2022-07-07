// Copyright 2022 3DTech, All rights reserved.

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
		FShaderOptimizationToolCommands::Get().PluginAction_SwitchLembertView,
		FExecuteAction::CreateRaw(this, &FShaderOptimizationToolModule::SwitchLembertView),
		FCanExecuteAction(),
		FIsActionChecked::CreateRaw(this, &FShaderOptimizationToolModule::IsSwitchLembertView));

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

void FShaderOptimizationToolModule::SwitchLembertView()
{
	FShaderCompilingManagerHelper::SwitchLembertView();
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
							LOCTEXT("SkipShaderCompilation", "Can skip scene material compilation, but do not open the material editor after enabling the skip material compilation function, and do not compile it if it is already open"),
							FSlateIcon(TEXT("EditorStyle"), "MaterialEditor.Apply")
						));
					Entry.SetCommandList(CommandListRef);
				}

				{
					FToolMenuEntry& Entry = Section.AddEntry(
						FToolMenuEntry::InitToolBarButton(
							FShaderOptimizationToolCommands::Get().PluginAction_SwitchLembertView,
							FText::FromString("SwitchLembertView"),
							LOCTEXT("SwitchLembertView", "Turn on to turn the scene into a grayscale effect, turn it off to restore the default display effect, and the selected object will not turn into a grayscale effect"),
							FSlateIcon(TEXT("EditorStyle"), "LandscapeEditor.RampTool")
						));
					Entry.SetCommandList(CommandListRef);
				}

				{
					FToolMenuEntry& Entry = Section.AddEntry(
						FToolMenuEntry::InitToolBarButton(
							FShaderOptimizationToolCommands::Get().PluginAction_CancelAllCompilations,
							FText::FromString("CancelAllCompilations"),
							LOCTEXT("CancelAllCompilations", "Close the current shader compilation queue"),
							FSlateIcon(TEXT("EditorStyle"), "ContentReference.Clear")
						));
					Entry.SetCommandList(CommandListRef);
				}

				{
					FToolMenuEntry& Entry = Section.AddEntry(
						FToolMenuEntry::InitToolBarButton(
							FShaderOptimizationToolCommands::Get().PluginAction_ReCompilations,
							FText::FromString("ReCompilations"),
							LOCTEXT("ReCompilations", "You can select or recompile the shader for the entire scene"),
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

bool FShaderOptimizationToolModule::IsSwitchLembertView() const
{
	return FShaderCompilingManagerHelper::IsSwitchLembertView();
}

void FShaderOptimizationToolModule::OnToggleSkipShaderCompilation()
{
	FShaderCompilingManagerHelper::OnToggleSkipShaderCompilation();
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FShaderOptimizationToolModule, ShaderOptimizationTool)