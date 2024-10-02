// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BIY_GameMode.h"
#include "BabaPlayerController.h"

ABIY_GameMode::ABIY_GameMode()
{
	PlayerControllerClass = ABabaPlayerController::StaticClass();
	MoveStacIndex = 0;
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
			BabaObjectsStatesMap.Add(AsBabaObject, FBabaObjectStateWrapper(AsBabaObject));//saves initial states
		}
	}
}

void ABIY_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*for (TPair<ABaseBabaObject*, FBabaObjectStateWrapper>& KVP : BabaObjectsStatesMap)
	{
		FBabaObjectState PreviousState = KVP.Value.GetBabaObjectState();
		DebugPreviusLocation(PreviousState.ObjectLocation, KVP.Value.Values.Num(), PreviousState.ObjectVisual);
	}*/
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

//To do : Optimise.
void ABIY_GameMode::TryUpdateBabaGameState()
{

	TArray<ABaseBabaObject*> BabaObjects;
	BabaObjectsStatesMap.GenerateKeyArray(BabaObjects);
	for (auto itr : BabaObjects)
	{
		BabaObjectsStatesMap[itr].RecordBabaObject(itr);
	}

	MoveStacIndex++;
}

//To do : Optimise.
void ABIY_GameMode::UndoMove()
{
	for (TPair<ABaseBabaObject*, FBabaObjectStateWrapper>& KVP : BabaObjectsStatesMap)
	{
		FBabaObjectState PreviousState = KVP.Value.GetBabaObjectState();

		if(PreviousState.bIsValidState && PreviousState.ObjectVisual)
		   PreviousState.UpdateBabaWithStruct(KVP.Key);

		DebugPreviusLocation(PreviousState.ObjectLocation, KVP.Value.Values.Num(), PreviousState.ObjectVisual);
	}
}
