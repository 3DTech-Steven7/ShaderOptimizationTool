// Copyright 2022 3DTech, All rights reserved.

#pragma once
#include "CoreMinimal.h"


class FShaderCompilingManagerHelper
{
public:
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

	static TMap< AActor*, TMap<class UActorComponent*, TArray<TObjectPtr<class UMaterialInterface>>>> LastActorComponentOverrideMaterials;
	
	static TMap< AActor*, TArray<TObjectPtr<class UMaterialInterface>>> LastActorOverrideMaterials;

	static UMaterialInterface* LoadSafeParentMaterial();
};