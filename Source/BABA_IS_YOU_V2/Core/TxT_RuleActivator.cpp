// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "TxT_RuleActivator.h"
#include "BIY_GameMode.h"

/*to increase readablity*/
#define VIRTICAL true
#define HOREZINTAL false
#define RVERSESWITCH true
#define PREFORMSWITCH false
/*to increase readablity*/

void ATxT_RuleActivator::TxTDoYourThing(EPushDirection ChangeDirection)
{
	CallsCount++;
	UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR CALLS : called %d times"), CallsCount);

	if (UpperTarget && UpDown_SwitchedToType)
	{
		PreformObjectTypeSwitch(UpDown_SwitchedToType, UpperTarget->Target, VIRTICAL, RVERSESWITCH);
		BottomRule = nullptr;
		UpperTarget = nullptr;
		UpDown_SwitchedToType = nullptr;
	}

	if (LeftTarget && LeftRight_SwitchedToType)
	{
		PreformObjectTypeSwitch(LeftRight_SwitchedToType, LeftTarget->Target, HOREZINTAL, PREFORMSWITCH);
		RightRule = nullptr;
		LeftTarget = nullptr;
		LeftRight_SwitchedToType = nullptr;
	}

	FVector DummyVector = FVector();
	ATxT_RuleTarget* UpperGrid = Cast<ATxT_RuleTarget>(GetObjectInGrid(EPushDirection::UP, DummyVector));//since we only need the object and not wanting to move we dont need real vector
	ABaseBabaObject* LowerGrid =GetObjectInGrid(EPushDirection::Down, DummyVector);//since this may be a rule or another target for type swap it better to get it by the general type then cast later.

	ATxT_RuleTarget* LeftGrid = Cast<ATxT_RuleTarget>(GetObjectInGrid(EPushDirection::Left, DummyVector));
	ABaseBabaObject* RightGrid = GetObjectInGrid(EPushDirection::Right, DummyVector);

	bool ActivationState = false;
	ATxT_RuleTarget* VerticalTransformTarget;
	if (!(UpperTarget == UpperGrid && BottomRule == LowerGrid))//this checks if nothing changed we dont reactivate.
	{
		std::tie(ActivationState, VerticalTransformTarget) = TryActivate(UpperGrid, LowerGrid,true, [this](ATxT_RuleHolder* ruleHolder, ATxT_RuleTarget* target)
			{
				LastAppliedRuleToUpper = ruleHolder;
				BottomRule = ruleHolder;
				UpperTarget = target;
			});

		if (!ActivationState)
		{
			if (VerticalTransformTarget)
				PreformObjectTypeSwitch(UpperGrid->Target, VerticalTransformTarget->Target, VIRTICAL, PREFORMSWITCH);

			else if (BottomRule && UpperTarget)
			{
				UpperTarget->RemoveRuleFromTarget(BottomRule);
				BottomRule = nullptr;
				UpperTarget = UpperGrid;
			}

		}
	}

	ATxT_RuleTarget* HorizentalTransformTarget;
	if (!(LeftTarget == LeftGrid && RightRule == RightGrid))
	{
		std::tie(ActivationState, HorizentalTransformTarget) = TryActivate(LeftGrid, RightGrid,false, [this](ATxT_RuleHolder* ruleHolder, ATxT_RuleTarget* target)
			{
				LastAppliedRuleToLeft = ruleHolder;
				RightRule = ruleHolder;
				LeftTarget = target;
			});

		if (!ActivationState)
		{
			if (HorizentalTransformTarget)
				PreformObjectTypeSwitch(LeftGrid->Target, HorizentalTransformTarget->Target, HOREZINTAL, PREFORMSWITCH);

			else if (RightRule && LeftTarget)
			{
				LeftTarget->RemoveRuleFromTarget(RightRule);
				RightRule = nullptr;
				LeftTarget = LeftGrid;
			}

		}
	}
}

//moved here because i think the activator is the best place for this.
void ATxT_RuleActivator::PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> This, TSubclassOf<ABaseBabaObject> ToThis, bool bBetweenUpDownGrids, bool ReverseTypeSwitch)
{
	UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Transformation Complete!"));

	TSubclassOf<ABaseBabaObject>& SwapContext = bBetweenUpDownGrids? UpDown_SwitchedToType : LeftRight_SwitchedToType;
	SwapContext = ToThis;

	Internal_PreformObjectTypeSwitch(This, ToThis, ReverseTypeSwitch);
}

void ATxT_RuleActivator::Internal_PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> This, TSubclassOf<ABaseBabaObject> ToThis, bool ReverseTypeSwitch)
{

	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//this template is used to feed the swapped object with the applied rule on it's type at the moment of spawning so it flows with the game current rules set.
	//ABaseBabaObject* NewTypeTemplate = Cast<ABaseBabaObject>(UGameplayStatics::GetActorOfClass(GetWorld(), ToThis)); not needed since we have the rules vs type map

	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), This, FoundObjects);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TArray< ABaseBabaObject*> NewObjects;
	
	if(GM)
	{
		FRuleTargets& AppliedRuleOnThisType = GM->GetRulesForObjectType(ToThis);

		for (auto& itr : FoundObjects)
		{
			if (ReverseTypeSwitch && !itr->ActorHasTag(ToThis->GetFName()))
				continue;

			ABaseBabaObject* SwappedObject = GetWorld()->SpawnActor<ABaseBabaObject>(ToThis, itr->GetActorTransform(), SpawnParams);
			FName ActorTag = This->GetFName();

			if(!ReverseTypeSwitch)
			   SwappedObject->Tags.Add(ActorTag);//we add the tag only when we are swapping type so we differ between orginal object type and transformed types.

			NewObjects.Add(SwappedObject);

			if(AppliedRuleOnThisType.bIsValid)
			{
				for (UBabaRule* rule : AppliedRuleOnThisType.AppliedRulesOnType)
				{
					SwappedObject->ApplyRuleOnObject(rule, AppliedRuleOnThisType.RuleID);
				}
			}

			GM->BabaObjectsInLevel.RemoveSingle(Cast<ABaseBabaObject>(itr));
			itr->Destroy();
		}


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

//to do , make it work with direction enum so it sets the refernce correctly since now the target checks for 4 activators so one boolean is not enough to make it right 
std::tuple<bool, ATxT_RuleTarget*> ATxT_RuleActivator::TryActivate(ATxT_RuleTarget* TargetGrid, ABaseBabaObject* AssumedRuleGrid, bool bHorizental, TFunction<void(ATxT_RuleHolder*, ATxT_RuleTarget*)> PostActivationEvent)
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
			if (bHorizental)
			{
				TargetGrid->LastAlingedActivatorBottom = this;
				AsRulegrid->LastAlingedActivatorUpper = this;
			}
			else
			{
				TargetGrid->LastAlingedActivatorRight = this;
				AsRulegrid->LastAlingedActivatorLeft = this;
			}
			return std::make_tuple(ActivateSucess, nullptr);
		}
		else
		{
			ATxT_RuleTarget* GridAstarget = Cast<ATxT_RuleTarget>(AssumedRuleGrid);
			PostActivationEvent(AsRulegrid, TargetGrid);
			return std::make_tuple(ActivateSucess, GridAstarget);
		}
	}

	return std::tuple<bool, ATxT_RuleTarget*>();
}