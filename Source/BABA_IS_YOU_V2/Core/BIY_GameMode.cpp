// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BIY_GameMode.h"
#include "BabaPlayerController.h"

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
		}
	}

	InitObjectsStates();//stores the initial states
}


void ABIY_GameMode::RegisterYouObject(ABaseBabaObject* newObject)
{
	   IsYouObjects.Add(newObject);
}

//probably will need to update to use classes 
void ABIY_GameMode::UnRegisterYouObject(ABaseBabaObject* Object)
{
	if (IsYouObjects.Contains(Object))
	{
		IsYouObjects.Remove(Object); 
	}
	if (IsYouObjects.Num() == 0)
	{
		BabaGameFinished(false);
	}
}

void ABIY_GameMode::RegisterTargetForRule(UBabaRule* rule, ABaseBabaObject* target)
{
	if (Rules_Targets_Map.Contains(rule))
	{
		Rules_Targets_Map[rule].RuleTargets.Add(target);
	}
	else
	{
		FRuleTargets targets;
		targets.RuleTargets.Add(target);
		Rules_Targets_Map.Add(rule, targets);
	}
	
}

void ABIY_GameMode::UnRegisterTargetFormRule(UBabaRule* rule, ABaseBabaObject* target)
{
	if (Rules_Targets_Map.Contains(rule))
	{
		Rules_Targets_Map[rule].RuleTargets.Remove(target);
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
}

void ABIY_GameMode::InitObjectsStates()
{
	for (auto& itr : BabaObjectsInLevel)
	{
		itr->CachedObjectState.UpdateStructWithBaba(itr);
	}
}
