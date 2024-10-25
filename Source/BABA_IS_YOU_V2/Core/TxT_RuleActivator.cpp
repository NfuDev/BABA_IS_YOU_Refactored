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
	bool bIsVerticalMovement = (ChangeDirection == EPushDirection::UP || ChangeDirection == EPushDirection::Down);
	bool bIsHorizentalMovement = (ChangeDirection == EPushDirection::Left || ChangeDirection == EPushDirection::Right);

	if (UpperTarget && UpDown_SwitchedToType && bIsHorizentalMovement)
	{
		UpperTarget->ContradictionVisuals->SetVisibility(false);

		if(VerticalTransformTarget)
			VerticalTransformTarget->ContradictionVisuals->SetVisibility(false);

		PreformObjectTypeSwitch(UpDown_SwitchedToType, UpperTarget->Target, VIRTICAL, RVERSESWITCH,nullptr);
		BottomRule = nullptr;
		UpperTarget = nullptr;
		UpDown_SwitchedToType = nullptr;
	}

	if (LeftTarget && LeftRight_SwitchedToType && bIsVerticalMovement)
	{
		LeftTarget->ContradictionVisuals->SetVisibility(false);

		if (HorizentalTransformTarget)
			HorizentalTransformTarget->ContradictionVisuals->SetVisibility(false);

		PreformObjectTypeSwitch(LeftRight_SwitchedToType, LeftTarget->Target, HOREZINTAL, RVERSESWITCH,nullptr);//we dont care about transformation target if we were reversing transform
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
	if (!(UpperTarget == UpperGrid && BottomRule == LowerGrid) && bIsHorizentalMovement)//this checks if nothing changed we dont reactivate.
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
				PreformObjectTypeSwitch(UpperGrid->Target, VerticalTransformTarget->Target, VIRTICAL, PREFORMSWITCH, VerticalTransformTarget);

			else if (BottomRule && UpperTarget)
			{
				UpperTarget->RemoveRuleFromTarget(BottomRule);
				BottomRule = nullptr;
				UpperTarget = UpperGrid;
			}

		}
	}

	if (!(LeftTarget == LeftGrid && RightRule == RightGrid) && bIsVerticalMovement)
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
				PreformObjectTypeSwitch(LeftGrid->Target, HorizentalTransformTarget->Target, HOREZINTAL, PREFORMSWITCH, HorizentalTransformTarget);

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
void ATxT_RuleActivator::PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> This, TSubclassOf<ABaseBabaObject> ToThis, bool bBetweenUpDownGrids, bool ReverseTypeSwitch, ATxT_RuleTarget* TransformationTarget)
{

	TSubclassOf<ABaseBabaObject>& SwapContext = bBetweenUpDownGrids? UpDown_SwitchedToType : LeftRight_SwitchedToType;
	SwapContext = ToThis;

	Internal_PreformObjectTypeSwitch(This, ToThis, bBetweenUpDownGrids, ReverseTypeSwitch, TransformationTarget);
}

void ATxT_RuleActivator::Internal_PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> This, TSubclassOf<ABaseBabaObject> ToThis, bool bBetweenUpDownGrids, bool ReverseTypeSwitch, ATxT_RuleTarget* TransformationTarget)
{

	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//this template is used to feed the swapped object with the applied rule on it's type at the moment of spawning so it flows with the game current rules set.
	//ABaseBabaObject* NewTypeTemplate = Cast<ABaseBabaObject>(UGameplayStatics::GetActorOfClass(GetWorld(), ToThis)); not needed since we have the rules vs type map

	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), This, FoundObjects);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TArray< ABaseBabaObject*> NewObjects;

	//if it has a tag of it self it means it is locked with OR the obejct is dead so we prevent the switch.
	if (FoundObjects.Num() > 0)
	{
		FName ActorTag = FoundObjects[0]->GetClass()->GetFName();
		ABaseBabaObject* AsBabaObject = Cast<ABaseBabaObject>(FoundObjects[0]);

		if (AsBabaObject->BabaObjectState == EBabaObjectState::Dead)
			return;
	
		if (AsBabaObject->TransformationTag == ActorTag && !ReverseTypeSwitch)
		{
			//contradction!!!.
			UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Transformation Contradicted!"));
			if (bBetweenUpDownGrids)
			{
				if (UpperTarget)
				{
					UpperTarget->ContradictionVisuals->SetVisibility(true);
				}

				if (TransformationTarget)
				{
					TransformationTarget->ContradictionVisuals->SetVisibility(true);
				}
			}
			else
			{
				if (LeftTarget)
				{
					LeftTarget->ContradictionVisuals->SetVisibility(true);
				}

				if (TransformationTarget)
				{
					TransformationTarget->ContradictionVisuals->SetVisibility(true);
				}
			}
			return;
		}
	}

	if(GM)
	{
		FRuleTargets& AppliedRuleOnThisType = GM->GetRulesForObjectType(ToThis);

		for (auto& itr : FoundObjects)
		{
			if (ReverseTypeSwitch && !itr->ActorHasTag(ToThis->GetFName()))
				continue;

			ABaseBabaObject* SwappedObject = GetWorld()->SpawnActor<ABaseBabaObject>(ToThis, itr->GetActorTransform(), SpawnParams);
			FName ActorTag = This->GetFName();

			
			if (!ReverseTypeSwitch)
			{
				SwappedObject->Tags.Add(ActorTag);//we add the tag only when we are swapping type so we differ between orginal object type and transformed types.
				SwappedObject->TransformationTag = ActorTag;
			}
			  

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

		if(NewObjects.Num() > 0)
			UE_LOG(LogTemp, Warning, TEXT("ACTIVATOR : Transformation Complete!"));

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

//####################################################################################
//########## AND OPERATOR ############################################################

void ATxT_AND_Activator::TxTDoYourThing(EPushDirection ChangeDirection)
{
	UE_LOG(LogTemp, Warning, TEXT("THEAND OPERATOR : and is Called"));

	FVector DummyVector = FVector();
	ABabaTextObjectBase* UpperGrid = Cast<ABabaTextObjectBase>(GetObjectInGrid(EPushDirection::UP, DummyVector));
	ABabaTextObjectBase* LowerGrid = Cast<ABabaTextObjectBase>(GetObjectInGrid(EPushDirection::Down, DummyVector));

	ABabaTextObjectBase* LeftGrid = Cast<ABabaTextObjectBase>(GetObjectInGrid(EPushDirection::Left, DummyVector));
	ABabaTextObjectBase* RightGrid = Cast<ABabaTextObjectBase>(GetObjectInGrid(EPushDirection::Right, DummyVector));

	if (LeftGrid && RightGrid)
	{
		ATxT_RuleTarget* AsSecondaryTarget = Cast<ATxT_RuleTarget>(LeftGrid);
		ATxT_RuleTarget* AsMainTarget = Cast<ATxT_RuleTarget>(RightGrid);

		if (AsSecondaryTarget && AsMainTarget)
		{
			if(HorezintalOperatedTarget != AsMainTarget)
			{

				AsMainTarget->AdditionalTargets.Add(AsSecondaryTarget->Target);
				for (auto& itr : AsSecondaryTarget->AdditionalTargets)
				{
					AsMainTarget->AdditionalTargets.Add(itr);
				}
				UE_LOG(LogTemp, Warning, TEXT("THEAND OPERATOR : Added %s additional target on %s "), *AsSecondaryTarget->Target->GetName(), *AsMainTarget->GetName());
			}
		}

		if (HorezintalOperatedTarget != AsMainTarget)
		{
			AsMainTarget->TryApplyOnAdditionalTargets();

			if (HorezintalOperatedTarget)
			{
				HorezintalOperatedTarget->RemoveAdditionalTargets();
			}

			HorezintalOperatedTarget = AsMainTarget;
		}
		  
	}

	else
	{
		if (HorezintalOperatedTarget)
		{
			HorezintalOperatedTarget->RemoveAdditionalTargets();
			HorezintalOperatedTarget = nullptr;
		}
	}

	if (UpperGrid && LowerGrid)
	{
		ATxT_RuleTarget* AsSecondaryTarget = Cast<ATxT_RuleTarget>(UpperGrid);
		ATxT_RuleTarget* AsMainTarget = Cast<ATxT_RuleTarget>(LowerGrid);

		if (AsSecondaryTarget && AsMainTarget)
		{
			if(VerticalOperatedTarget != AsMainTarget)
			{
				AsMainTarget->AdditionalTargets.Add(AsSecondaryTarget->Target);
				for (auto& itr : AsSecondaryTarget->AdditionalTargets)
				{
					AsMainTarget->AdditionalTargets.Add(itr);
				}

				UE_LOG(LogTemp, Warning, TEXT("THEAND OPERATOR : Added %s additional target on %s "), *AsSecondaryTarget->Target->GetName(), *AsMainTarget->GetName());
			}
		}

		if (VerticalOperatedTarget != AsMainTarget)
		{
			AsMainTarget->TryApplyOnAdditionalTargets();

			if (VerticalOperatedTarget)
			{
				VerticalOperatedTarget->RemoveAdditionalTargets();
			}

			VerticalOperatedTarget = AsMainTarget;
		}
	
	}

	else
	{
		if (VerticalOperatedTarget)
		{
			VerticalOperatedTarget->RemoveAdditionalTargets();
			VerticalOperatedTarget = nullptr;
		}
	}

}

void ATxT_AND_Activator::PostUndo()
{
	TxTDoYourThing(EPushDirection());
}
