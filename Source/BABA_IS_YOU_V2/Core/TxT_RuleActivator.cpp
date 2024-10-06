// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "TxT_RuleActivator.h"
#include "BIY_GameMode.h"


void ATxT_RuleActivator::TxTDoYourThing(EPushDirection ChangeDirection)
{
	CallsCount++;
	UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR CALLS : called %d times"), CallsCount);

	FVector DummyVector = FVector();
	ATxT_RuleTarget* UpperGrid = Cast<ATxT_RuleTarget>(GetObjectInGrid(EPushDirection::UP, DummyVector));//since we only need the object and not wanting to move we dont need real vector
	ABaseBabaObject* LowerGrid =GetObjectInGrid(EPushDirection::Down, DummyVector);//since this may be a rule or another target for type swap it better to get it by the general type then cast later.

	ATxT_RuleTarget* LeftGrid = Cast<ATxT_RuleTarget>(GetObjectInGrid(EPushDirection::Left, DummyVector));
	ABaseBabaObject* RightGrid = GetObjectInGrid(EPushDirection::Right, DummyVector);

	bool ActivationState = false;
	ATxT_RuleTarget* VerticalTransformTarget;

	std::tie(ActivationState, VerticalTransformTarget) = TryActivate(UpperGrid, LowerGrid,[this](ATxT_RuleHolder* ruleHolder, ATxT_RuleTarget* target)
		{
			LastAppliedRuleToUpper = ruleHolder;
			BottomRule = ruleHolder;
			UpperTarget = target;
		});

	if (!ActivationState)
	{
		if (VerticalTransformTarget)
			PreformObjectTypeSwitch(UpperGrid->Target, VerticalTransformTarget->Target, true);
			  
		else if (BottomRule && UpperTarget)
		{
			UpperTarget->RemoveRuleFromTarget(BottomRule);
			BottomRule = nullptr;
			UpperTarget = nullptr;
		}
			
	}

	ATxT_RuleTarget* HorizentalTransformTarget;

	std::tie(ActivationState, HorizentalTransformTarget) = TryActivate(LeftGrid, RightGrid,[this](ATxT_RuleHolder* ruleHolder, ATxT_RuleTarget* target)
		{
			LastAppliedRuleToLeft = ruleHolder;
			RightRule = ruleHolder;
			LeftTarget = target;
		});

	if (!ActivationState)
	{
		if(HorizentalTransformTarget)
		  PreformObjectTypeSwitch(LeftGrid->Target, HorizentalTransformTarget->Target, false);

		else if (RightRule && LeftTarget)
			{
			   LeftTarget->RemoveRuleFromTarget(RightRule);
			   LeftTarget = nullptr;
			   RightRule = nullptr;
			}
			 
	}
}

//moved here because i think the activator is the best place for this.
void ATxT_RuleActivator::PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> This, TSubclassOf<ABaseBabaObject> ToThis, bool bBetweenUpDownGrids)
{
	UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Transformation Complete!"));

	//TSubclassOf<ABaseBabaObject>& SwapContext = bBetweenUpDownGrids? UpDown_SwitchedToType : LeftRight_SwitchedToType;
	//SwapContext = ToThis;

	//Internal_PreformObjectTypeSwitch(This, ToThis);
}

void ATxT_RuleActivator::Internal_PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> This, TSubclassOf<ABaseBabaObject> ToThis)
{
	//this template is used to feed the swapped object with the applied rule on it's type at the moment of spawning so it flows with the game current rules set.
	ABaseBabaObject* NewTypeTemplate = Cast<ABaseBabaObject>(UGameplayStatics::GetActorOfClass(GetWorld(), ToThis));

	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), This, FoundObjects);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TArray< ABaseBabaObject*> NewObjects;

	for (auto& itr : FoundObjects)
	{
		ABaseBabaObject* SwappedObject = GetWorld()->SpawnActor<ABaseBabaObject>(ToThis, itr->GetActorTransform(), SpawnParams);
		NewObjects.Add(SwappedObject);

		if (NewTypeTemplate)
		{
			for (UBabaRule* rule : NewTypeTemplate->AppliedRules)
			{
				SwappedObject->ApplyRuleOnObject(rule);
			}
		}
		else
		{
			// find active rules in the world then apply them here.
		}

		itr->Destroy();
	}

	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GM)
	{
		for (auto& itr : NewObjects)
		{
			itr->RegisterNeighbours();
			itr->CachedObjectState.UpdateStructWithBaba(itr);
			itr->PostUndo();
			GM->BabaObjectsInLevel.Add(itr);
		}

		//GM->EvaluateAllRules();
		GM->CheckGameFinished();//in case of swapping( Is you ) object with another none ( is you ) object.
	}
}

std::tuple<bool, ATxT_RuleTarget*> ATxT_RuleActivator::TryActivate(ATxT_RuleTarget* TargetGrid, ABaseBabaObject* AssumedRuleGrid, TFunction<void(ATxT_RuleHolder*, ATxT_RuleTarget*)> PostActivationEvent)
{
	bool ActivateSucess = false;

	if (TargetGrid && AssumedRuleGrid)
	{
		ATxT_RuleHolder* AsRulegrid = Cast<ATxT_RuleHolder>(AssumedRuleGrid);
		if (AsRulegrid)
		{
			TargetGrid->ApplyRuleOnTarget(AsRulegrid);
			ActivateSucess = true;
			PostActivationEvent(AsRulegrid, TargetGrid);
			return std::make_tuple(ActivateSucess, nullptr);
		}
		else
		{
			ATxT_RuleTarget* GridAstarget = Cast<ATxT_RuleTarget>(AssumedRuleGrid);
			return std::make_tuple(ActivateSucess, GridAstarget);
		}
	}

	return std::tuple<bool, ATxT_RuleTarget*>();
}

/*
* if (UpperGrid && LowerGrid)
	{
		ATxT_RuleTarget* GridAstarget = Cast<ATxT_RuleTarget>(LowerGrid);

		if(UpperGrid != UpperTarget)
		{
			BottomRule = Cast<ATxT_RuleHolder>(LowerGrid);
			UpperTarget = UpperGrid;
			if (BottomRule)
			{
				if (IsValid(UpperTarget->Target) && IsValid(BottomRule->Rule))
				{
					//apply the rule on the target
					UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : trying to apply rule %s On %s"), *BottomRule->Rule->GetName(), *UpperTarget->Target->GetName());
					UpperTarget->ApplyRuleOnTarget(BottomRule);
					LastAppliedRuleToUpper = BottomRule;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : trying to Activate Invalid Objects"));
				}
			}

			else if (GridAstarget)
			{
				UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Preform Type Swap from %s to %s"), *UpperTarget->Target->GetName(), *GridAstarget->Target->GetName());
				PreformObjectTypeSwitch(UpperTarget->Target, GridAstarget->Target, true);
			}
		}
	}

	else
	{
		if (UpperTarget && LastAppliedRuleToUpper)
		{
			UpperTarget->RemoveRuleFromTarget(LastAppliedRuleToUpper);
			LastAppliedRuleToUpper = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Rule Removed!!"));
		}
		else if (UpDown_SwitchedToType && UpperTarget)
		{
			TSubclassOf<ABaseBabaObject> TempClass = UpDown_SwitchedToType;
			UpDown_SwitchedToType = NULL;
			Internal_PreformObjectTypeSwitch(TempClass, UpperTarget->Target);
			UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Restored Transformation from %s"), *UpDown_SwitchedToType->GetName());

		}
		if (!UpperGrid)
			UpperTarget = nullptr;
	}

	if (LeftGrid && RightGrid)
	{
		if(LeftGrid != LeftTarget)
		{
			RightRule = Cast<ATxT_RuleHolder>(RightGrid);
			LeftTarget = LeftGrid;
			ATxT_RuleTarget* GridAstarget = Cast<ATxT_RuleTarget>(RightGrid);

			if (RightRule)
			{
				if (IsValid(LeftTarget->Target) && IsValid(RightRule->Rule))
				{
					//apply the rule on the target
					UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : trying to apply rule %s On %s"), *RightRule->Rule->GetName(), *LeftTarget->Target->GetName());
					LeftTarget->ApplyRuleOnTarget(RightRule);
					LastAppliedRuleToLeft = RightRule;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : trying to Activate Invalid Objects"));
				}
			}
			else if (GridAstarget)
			{
				UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Preform Type Swap from %s to %s"), *LeftTarget->Target->GetName(), *GridAstarget->Target->GetName());
				PreformObjectTypeSwitch(LeftTarget->Target, GridAstarget->Target, false);
			}
		}
	}
	else
	{
		if (LeftTarget && LastAppliedRuleToLeft)
		{
			LeftTarget->RemoveRuleFromTarget(LastAppliedRuleToLeft);
			LastAppliedRuleToLeft = nullptr;
		}
		else if (LeftRight_SwitchedToType && LeftTarget)
		{
			TSubclassOf<ABaseBabaObject> TempClass = LeftRight_SwitchedToType;
			LeftRight_SwitchedToType = NULL;
			Internal_PreformObjectTypeSwitch(TempClass, LeftTarget->Target);
			UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Restored Transformation from %s"), *LeftRight_SwitchedToType->GetName());
		}
		if (!LeftGrid)
			LeftTarget = nullptr;
	}
*/