// Copyright 2022 3DTech, All rights reserved.

#include "ShaderOptimizationToolStyle.h"
#include "ShaderOptimizationTool.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FShaderOptimizationToolStyle::StyleInstance = nullptr;

void FShaderOptimizationToolStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FShaderOptimizationToolStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FShaderOptimizationToolStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ShaderOptimizationToolStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FShaderOptimizationToolStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("ShaderOptimizationToolStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ShaderOptimizationTool")->GetBaseDir() / TEXT("Resources"));
	return Style;
}

void FShaderOptimizationToolStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FShaderOptimizationToolStyle::Get()
{
	return *StyleInstance;
}
