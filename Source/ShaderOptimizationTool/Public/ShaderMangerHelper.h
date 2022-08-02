// Copyright 2022 3DTech, All rights reserved.

#pragma once
#include "CoreMinimal.h"


class FShaderCompilingManagerHelper
{
public:
	static void SetSkipShaderCompilation(bool bInSkip);
	
	static void OnToggleSkipShaderCompilation();

	static void ReCompilations();

	static void SwitchOverrideMaterial(bool bOverlay);

	static void SwitchLembertView();

	static void ReSetWorldMaterials();

	static void GetMaterialSet(TSet<UMaterialInterface*>& MaterialInterfaceSet, TSet<UMaterialInterface*>& SelActorMaterialSet);

	static void GetMaterialInstance(UMaterialInstance*& MaterialInterface, TSet<UMaterialInstance*>& MaterialInterfaceSet);

	static void CancelAllCompilations();

	static bool CheckCanCancel();

	static bool IsShaderCompilationSkipped();

	static bool IsSwitchLembertView();

	static bool IsGameWorld();

	static bool bOverlayMaterial;

	static TMap< AActor*, TMap<class UActorComponent*, TArray<class UMaterialInterface*>>> LastActorComponentOverrideMaterials;
	
	static TMap< AActor*, TArray<class UMaterialInterface*>> LastActorOverrideMaterials;

	static UMaterial* LoadSafeParentMaterial();
	
	static TAutoConsoleVariable<int32> CVarSkipShaderCompilation;
};