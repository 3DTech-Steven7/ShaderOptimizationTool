// Copyright 2022 3DTech, All rights reserved.

#include "ShaderMangerHelper.h"
#include <ShaderCompiler.h>
#include "LandscapeProxy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 2
#include "MaterialDomain.h"
#else
#include "GlobalShader.h"
#endif


bool FShaderCompilingManagerHelper::bOverlayMaterial = false;

static const FString M_Lambert = TEXT("/ShaderOptimizationTool/M_Lambert");

TMap< AActor*, TMap<UActorComponent*, TArray<TObjectPtr<UMaterialInterface>>>> FShaderCompilingManagerHelper::LastActorComponentOverrideMaterials;
TMap< AActor*, TArray<TObjectPtr<UMaterialInterface>>> FShaderCompilingManagerHelper::LastActorOverrideMaterials;

TAutoConsoleVariable<int32> FShaderCompilingManagerHelper::CVarSkipShaderCompilation = TAutoConsoleVariable(
	TEXT("r.SkipShaderCompilation"),
	0,
	TEXT("Skip Shader Compilation.\n")
	TEXT("Note: Needs off == 0 && on == 1"),
	ECVF_Default);

void FShaderCompilingManagerHelper::SetSkipShaderCompilation(bool bInSkip)
{
	GShaderCompilingManager->SkipShaderCompilation(bInSkip);
}

void FShaderCompilingManagerHelper::OnToggleSkipShaderCompilation()
{
	if (IsShaderCompilationSkipped())
	{
		GShaderCompilingManager->SkipShaderCompilation(false);
	}
	else
	{
		GShaderCompilingManager->SkipShaderCompilation(true);
	}
}

void FShaderCompilingManagerHelper::ReCompilations()
{
	bool bShaderCompilationSkipped = IsShaderCompilationSkipped();
	if (bShaderCompilationSkipped)
	{
		OnToggleSkipShaderCompilation();
	}

	UE_LOG(LogTemp, Log, TEXT("ReCompilations"));

	TSet<UMaterialInterface*> MaterialInterfaceSet;
	TSet<UMaterialInterface*> SelActorMaterialSet;
	TSet<UMaterialInterface*> RecompileMateriaSet;
	TSet<UMaterial*> MaterialSet;

	GetMaterialSet(MaterialInterfaceSet, SelActorMaterialSet);

	if (SelActorMaterialSet.IsEmpty())
	{
		RecompileMateriaSet = MaterialInterfaceSet;
	}
	else
	{
		RecompileMateriaSet = SelActorMaterialSet;
	}

	TSet<UMaterialInstance*> RecompileMaterialInterfaceSet;

	for (auto& MaterialInterface : RecompileMateriaSet)
	{
		UMaterial* Material = nullptr;

		if (UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(MaterialInterface))
		{
			RecompileMaterialInterfaceSet.Emplace(MaterialInstance);
			GetMaterialInstance(MaterialInstance, RecompileMaterialInterfaceSet);
			Material = MaterialInstance->GetMaterial();
		}
		else
		{
			Material = Cast<UMaterial>(MaterialInterface);
		}

		if (Material)
		{
			MaterialSet.Emplace(Material);
		}
	}

	for (UMaterial*& Material : MaterialSet)
	{
		if (Material->IsDefaultMaterial())
		{
			continue;
		}
		Material->UpdateMaterialShaderCacheAndTextureReferences();
	}

	for (UMaterialInstance*& It : RecompileMaterialInterfaceSet)
	{
		It->ForceRecompileForRendering();
	}

	if (bShaderCompilationSkipped)
	{
		OnToggleSkipShaderCompilation();
	}

	UE_LOG(LogTemp, Log, TEXT("RecompileMateriaSet: %d"), RecompileMateriaSet.Num());
}

void FShaderCompilingManagerHelper::SwitchLembertView()
{
	if (IsGameWorld())
	{
		return;
	}
	
	if (bOverlayMaterial)
	{
		bOverlayMaterial = false;
		ReSetWorldMaterials();
	}
	else
	{
		SwitchOverrideMaterial(true);
		bOverlayMaterial = true;
		FTimerHandle TimeHandle;
		FTimerDelegate ExplosionDelegate = FTimerDelegate::CreateLambda([&TimeHandle]()
			{
				SwitchOverrideMaterial(false);
				bOverlayMaterial = true;
				GWorld->GetTimerManager().ClearTimer(TimeHandle);
			});

		GWorld->GetTimerManager().SetTimer(TimeHandle, ExplosionDelegate, 0.01f, false);
	}

	UE_LOG(LogTemp, Log, TEXT("Celar Cache"));
}

void FShaderCompilingManagerHelper::SwitchOverrideMaterial(bool bOverlay)
{
	UMaterial* SafeParent = LoadSafeParentMaterial();

	if (bOverlay)
	{
		LastActorOverrideMaterials.Reset();
		LastActorComponentOverrideMaterials.Reset();
	}

	UE_LOG(LogTemp, Log, TEXT("LastActorOverrideMaterials: %d"), LastActorOverrideMaterials.Num());
	for (int32 LevelIndex = 0; LevelIndex < GWorld->GetNumLevels(); LevelIndex++)
	{
		ULevel* Level = GWorld->GetLevel(LevelIndex);

		for (AActor* Actor : Level->Actors)
		{
			if (!Actor->IsValidLowLevel() || Actor->IsSelected())
			{
				continue;
			}

			if (ALandscapeProxy* LandscapeProxy = Cast<ALandscapeProxy>(Actor))
			{
				if (bOverlay)
				{
					LastActorOverrideMaterials.Add(LandscapeProxy, TArray<TObjectPtr<UMaterialInterface>>({LandscapeProxy->LandscapeMaterial}));
					LandscapeProxy->LandscapeMaterial = SafeParent;
					LandscapeProxy->UpdateAllComponentMaterialInstances(true);
				}
				else
				{
					if (TArray<TObjectPtr<UMaterialInterface>>* OverrideMaterials = LastActorOverrideMaterials.Find(LandscapeProxy))
					{
						LandscapeProxy->LandscapeMaterial = (*OverrideMaterials)[0];
					}
				}
				continue;
			}
			
			TArray<UActorComponent*> Components;

			Actor->GetComponents(Components);

			TMap<UActorComponent*, TArray<TObjectPtr<UMaterialInterface>>> ActorComponentOverrideMaterials;

			if (!bOverlay)
			{
				TMap<UActorComponent*, TArray<TObjectPtr<UMaterialInterface>>>* ActorOverrideMaterialCache = LastActorComponentOverrideMaterials.Find(Actor);
				if (ActorOverrideMaterialCache)
				{
					ActorComponentOverrideMaterials = *ActorOverrideMaterialCache;
				}
				else
				{
					continue;
				}
			}

			for (auto& ActorComponent : Components)
			{			
				if (Cast<USkinnedMeshComponent>(ActorComponent))
				{
					continue;
				}
				
				else if (UMeshComponent* MeshComponent = Cast<UMeshComponent>(ActorComponent))
				{
					if (bOverlay)
					{
						ActorComponentOverrideMaterials.Add(ActorComponent, MeshComponent->OverrideMaterials);
						for (int32 MaterialIndex = 0; MaterialIndex < MeshComponent->GetNumMaterials(); MaterialIndex++)
						{
							MeshComponent->SetMaterial(MaterialIndex, SafeParent);
						}
					}
					else
					{
						if (TArray<TObjectPtr<UMaterialInterface>>* OverrideMaterials = ActorComponentOverrideMaterials.Find(ActorComponent))
						{
							MeshComponent->OverrideMaterials = *OverrideMaterials;
						}
					}
				}
				
				else if (UParticleSystemComponent* ParticleSystemComponent = Cast<UParticleSystemComponent>(ActorComponent))
				{
					if (bOverlay)
					{
						ActorComponentOverrideMaterials.Add(ActorComponent, ParticleSystemComponent->EmitterMaterials);
						for (int32 MaterialIndex = 0; MaterialIndex < ParticleSystemComponent->GetNumMaterials(); MaterialIndex++)
						{
							ParticleSystemComponent->SetMaterial(MaterialIndex, SafeParent);
						}
					}
					else
					{
						if (TArray<TObjectPtr<UMaterialInterface>>* OverrideMaterials = ActorComponentOverrideMaterials.Find(ActorComponent))
						{
							MeshComponent->OverrideMaterials = *OverrideMaterials;
						}
					}
				}
			}

			if (bOverlay)
			{
                if(!ActorComponentOverrideMaterials.IsEmpty())
                {
                	LastActorComponentOverrideMaterials.Add(Actor, ActorComponentOverrideMaterials);
                }
			}
		}
	}
}

void FShaderCompilingManagerHelper::ReSetWorldMaterials()
{
	UMaterialInterface* SafeParent = UMaterial::GetDefaultMaterial(MD_Surface);

	for (int32 LevelIndex = 0; LevelIndex < GWorld->GetNumLevels(); LevelIndex++)
	{
		ULevel* Level = GWorld->GetLevel(LevelIndex);
		for (AActor* Actor : Level->Actors)
		{
			if (!Actor->IsValidLowLevel())
			{
				continue;
			}

			if (ALandscapeProxy* LandscapeProxy = Cast<ALandscapeProxy>(Actor))
			{
				LandscapeProxy->UpdateAllComponentMaterialInstances();
				continue;
			}
			
			TArray<UActorComponent*> Components;
			
			Actor->GetComponents(Components);
			for (auto& ActorComponent : Components)
			{
				if (Cast<USkinnedMeshComponent>(ActorComponent))
				{
					continue;
				}
				
				else if (UMeshComponent* MeshComponent = Cast<UMeshComponent>(ActorComponent))
				{
					for (int32 MaterialIndex = 0; MaterialIndex < MeshComponent->GetNumMaterials(); ++MaterialIndex)
					{
						UMaterialInterface* MaterialInterface = MeshComponent->GetMaterial(MaterialIndex);
						MeshComponent->SetMaterial(MaterialIndex, SafeParent);
						MeshComponent->SetMaterial(MaterialIndex, MaterialInterface);
					}
				}

				else if (UParticleSystemComponent* ParticleSystemComponent = Cast<UParticleSystemComponent>(ActorComponent))
				{
					for (int32 MaterialIndex = 0; MaterialIndex < ParticleSystemComponent->GetNumMaterials(); ++MaterialIndex)
					{
						UMaterialInterface* MaterialInterface = ParticleSystemComponent->GetMaterial(MaterialIndex);
						ParticleSystemComponent->SetMaterial(MaterialIndex, SafeParent);
						ParticleSystemComponent->SetMaterial(MaterialIndex, MaterialInterface);
					}
				}
			}
		}
	}
}

void FShaderCompilingManagerHelper::GetMaterialSet(TSet<UMaterialInterface*>& MaterialInterfaceSet, TSet<UMaterialInterface*>& SelActorMaterialSet)
{
	for (int32 LevelIndex = 0; LevelIndex < GWorld->GetNumLevels(); LevelIndex++)
	{
		ULevel* Level = GWorld->GetLevel(LevelIndex);
		for (const AActor* Actor : Level->Actors)
		{
			if (!Actor->IsValidLowLevel())
			{
				continue;
			}
			TArray<UActorComponent*> Components;
			
			Actor->GetComponents(Components);
			for (auto& ActorComponent : Components)
			{
				if (UMeshComponent* MeshComponent = Cast<UMeshComponent>(ActorComponent))
				{
					for (int32 MaterialIndex = 0; MaterialIndex < MeshComponent->GetNumMaterials(); ++MaterialIndex)
					{
						UMaterialInterface* MaterialInterface = MeshComponent->GetMaterial(MaterialIndex);
						MaterialInterfaceSet.Emplace(MaterialInterface);
						if (Actor->IsSelected())
						{
							SelActorMaterialSet.Emplace(MaterialInterface);
						}
					}
				}

				else if (UParticleSystemComponent* ParticleSystemComponent = Cast<UParticleSystemComponent>(ActorComponent))
				{
					for (int32 MaterialIndex = 0; MaterialIndex < ParticleSystemComponent->GetNumMaterials(); ++MaterialIndex)
					{
						UMaterialInterface* MaterialInterface = ParticleSystemComponent->GetMaterial(MaterialIndex);
						MaterialInterfaceSet.Emplace(MaterialInterface);
						if (Actor->IsSelected())
						{
							SelActorMaterialSet.Emplace(MaterialInterface);
						}
					}
				}
			}
		}
	}
}

void FShaderCompilingManagerHelper::GetMaterialInstance(UMaterialInstance*& MaterialInterface, TSet<UMaterialInstance*>& MaterialInterfaceSet)
{
	UMaterialInterface* MaterialInterface_Parent = MaterialInterface->Parent;
	if (MaterialInterface_Parent == nullptr)
	{
		return;
	}

	if (UMaterialInstance* MaterialInstance_Parent = Cast<UMaterialInstance>(MaterialInterface_Parent))
	{
		MaterialInterfaceSet.Emplace(MaterialInstance_Parent);
		GetMaterialInstance(MaterialInstance_Parent, MaterialInterfaceSet);
	}
}

void FShaderCompilingManagerHelper::CancelAllCompilations()
{
	TArray<int32> JobsIdx;
	for (int32 MapIdx = 0; MapIdx < GShaderCompilingManager->GetNumPendingJobs(); MapIdx++)
	{
		JobsIdx.Add(MapIdx);
	}
	 
	GShaderCompilingManager->CancelCompilation(TEXT(""), JobsIdx);
	
	TSet<UMaterialInterface*> MaterialInterfaceSet;
	TSet<UMaterialInterface*> SelActorMaterialSet;
	TSet<UMaterial*> MaterialSet;

	GetMaterialSet(MaterialInterfaceSet, SelActorMaterialSet);

	for (auto& MaterialInterface : MaterialInterfaceSet)
	{
		UMaterial* Material = nullptr;

		if (UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(MaterialInterface))
		{
			Material = MaterialInstance->GetMaterial();
		}
		else
		{
			Material = Cast<UMaterial>(MaterialInterface);
		}

		if (Material)
		{
			MaterialSet.Emplace(Material);
		}
	}

	for (UMaterial*& Material : MaterialSet)
	{
		if (Material->IsDefaultMaterial())
		{
			continue;
		}
		
		Material->CancelOutstandingCompilation();
	}
}

bool FShaderCompilingManagerHelper::CheckCanCancel()
{
	return GShaderCompilingManager->IsCompiling();
}

bool FShaderCompilingManagerHelper::IsShaderCompilationSkipped()
{
	return GShaderCompilingManager->IsShaderCompilationSkipped();
}

bool FShaderCompilingManagerHelper::IsSwitchLembertView()
{
	return bOverlayMaterial;
}

bool FShaderCompilingManagerHelper::IsGameWorld()
{
	return GEditor->PlayWorld || GEditor->bIsSimulatingInEditor;
}

UMaterial* FShaderCompilingManagerHelper::LoadSafeParentMaterial()
{
	UMaterial* SafeParent = FindObject<UMaterial>(nullptr, *M_Lambert);
	if (SafeParent == nullptr)
	{
		SafeParent = LoadObject<UMaterial>(nullptr, *M_Lambert, nullptr, LOAD_DisableDependencyPreloading, nullptr);
		if (SafeParent == nullptr)
		{
			SafeParent = UMaterial::GetDefaultMaterial(MD_Surface);
		}
		else
		{
			SafeParent->AddToRoot();
			GShaderCompilingManager->FinishAllCompilation();
		}
		
}
	return SafeParent;
}


