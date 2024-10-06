// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BIY_GameMode.h"
#include "BabaPlayerController.h"
#include "TxT_RuleActivator.h"

ABIY_GameMode::ABIY_GameMode()
{
	PlayerControllerClass = ABabaPlayerController::StaticClass();
}

void ABIY_GameMode::BeginPlay()
{
	TArray<AActor*> BabaObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBabaObject::StaticClass(), BabaObjects);

	for (auto& babaObejct : BabaObjects)
	{
		ABaseBabaObject* AsBabaObject = Cast<ABaseBabaObject>(babaObejct);
		if (AsBabaObject)
		{
			AsBabaObject->RegisterNeighbours();
			BabaObjectsInLevel.Add(AsBabaObject);

			ATxT_RuleActivator* AsActivator = Cast<ATxT_RuleActivator>(AsBabaObject);
			if (AsActivator)
			{
				RuleActivatorsList.Add(AsActivator);
			}
		}
	}

	InitObjectsStates();//stores the initial states
}


void ABIY_GameMode::RegisterYouObject(ABaseBabaObject* newObject)
{
	   IsYouObjects.AddUnique(newObject);
}

//probably will need to update to use classes 
void ABIY_GameMode::UnRegisterYouObject(ABaseBabaObject* Object)
{
	//this for death by messing with the rules.
	if (IsYouObjects.Contains(Object))
	{
		IsYouObjects.Remove(Object);
	}
	if (IsYouObjects.Num() == 0)
	{
		BabaGameFinished(false);
	}
}

void ABIY_GameMode::OnYouObjectDied(ABaseBabaObject* Object)
{
	//for death by overlap with certain types 
	int32 DeadCount = 0;
	
	for (auto& itr : IsYouObjects)
	{
		if (Object->BabaObjectState == EBabaObjectState::Dead)
		{
			DeadCount++;
		}
	}

	if (DeadCount == IsYouObjects.Num())
	{
		BabaGameFinished(false);
	}
}

void ABIY_GameMode::RegisterTargetForRule(UBabaRule* rule, TSubclassOf<ABaseBabaObject> ObjectType)
{
	if (Map_RulesOnObjectType.Contains(ObjectType))
	{
		FString Results = "Rules Before Adding : \n";
		for (auto& itr : Map_RulesOnObjectType[ObjectType].AppliedRulesOnType)
		{
			Results += itr->GetName() + "\n";
		}
		UE_LOG(LogTemp, Warning, TEXT("DEBUG REGISTER : Added %s To Map, map had before : %s "),*rule->GetName(), *Results);

		Map_RulesOnObjectType[ObjectType].AppliedRulesOnType.Add(rule);
	}
	else
	{
		FRuleTargets targets;
		targets.AppliedRulesOnType.Add(rule);
		Map_RulesOnObjectType.Add(ObjectType, targets);
	}

}

void ABIY_GameMode::UnRegisterTargetFormRule(UBabaRule* rule, TSubclassOf<ABaseBabaObject> ObjectType)
{
	if (Map_RulesOnObjectType.Contains(ObjectType))
	{
		UE_LOG(LogTemp, Warning, TEXT("Removed Target From Rules Map"));
		Map_RulesOnObjectType[ObjectType].AppliedRulesOnType.RemoveSingle(rule);
	}
}

void ABIY_GameMode::TryUpdateBabaGameState()
{
	for (auto& itr : BabaObjectsInLevel)
	{
		itr->RecordBabaObjectState();
    }
}

void ABIY_GameMode::UndoMove()
{
	for (auto& itr : BabaObjectsInLevel)
	{
		itr->OnBabaUndo();
	}

	//this need to be called for all after all objcts finish undo, so we call it here and not in the undo function per object since that is not accurate
	for (auto& itr : BabaObjectsInLevel)
	{
		itr->PostUndo();
	}
}

void ABIY_GameMode::InitObjectsStates()
{
	for (auto& itr : BabaObjectsInLevel)
	{
		itr->CachedObjectState.UpdateStructWithBaba(itr);
		itr->PostUndo();//yes this is not undo but this forces the activators to try to activate the rules when the game starts.
	}
}

void ABIY_GameMode::EvaluateAllRules()
{
	for (auto& itr : RuleActivatorsList)
	{
		itr->PostUndo();
	}
}
