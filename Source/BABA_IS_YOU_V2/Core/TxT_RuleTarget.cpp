// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "TxT_RuleTarget.h"
#include "BIY_GameMode.h"
#include "TxT_RuleActivator.h"

//targets cares only about the right and bottom grids
void ATxT_RuleTarget::TxTDoYourThing(EPushDirection ChangeDirection)
{

	bool bIsVerticalMovement = (ChangeDirection == EPushDirection::UP || ChangeDirection == EPushDirection::Down);
	bool bIsHorizentalMovement = (ChangeDirection == EPushDirection::Left || ChangeDirection == EPushDirection::Right);

	if (LastAlingedActivatorRight && bIsVerticalMovement)
		LastAlingedActivatorRight->TxTDoYourThing(ChangeDirection);

	if (LastAlingedActivatorBottom && bIsHorizentalMovement)
		LastAlingedActivatorBottom->TxTDoYourThing(ChangeDirection);

	if (LastAlingedActivatorUp && bIsHorizentalMovement)
		LastAlingedActivatorUp->TxTDoYourThing(ChangeDirection);

	if (LastAlingedActivatorLeft && bIsVerticalMovement)
		LastAlingedActivatorLeft->TxTDoYourThing(ChangeDirection);


	//FVector DummyVector = FVector();
	// LastAlingedActivatorRight = Cast<ATxT_RuleActivator>(GetObjectInGrid(EPushDirection::Right, DummyVector));

	// LastAlingedActivatorBottom = Cast<ATxT_RuleActivator>(GetObjectInGrid(EPushDirection::Down, DummyVector));

	// LastAlingedActivatorUp = Cast<ATxT_RuleActivator>(GetObjectInGrid(EPushDirection::UP, DummyVector));

	// LastAlingedActivatorLeft = Cast<ATxT_RuleActivator>(GetObjectInGrid(EPushDirection::Left, DummyVector));


	//if (LastAlingedActivatorRight)
	//	LastAlingedActivatorRight->TxTDoYourThing(ChangeDirection);

	//if (LastAlingedActivatorBottom)
	//	LastAlingedActivatorBottom->TxTDoYourThing(ChangeDirection);

	//if (LastAlingedActivatorUp)
	//	LastAlingedActivatorUp->TxTDoYourThing(ChangeDirection);

	//if (LastAlingedActivatorLeft)
	//	LastAlingedActivatorLeft->TxTDoYourThing(ChangeDirection);
}

//add internal function to do this, and this function should for loop on targets and call the interal function on them
void ATxT_RuleTarget::ApplyRuleOnTarget(ATxT_RuleHolder* RuleHolder)
{

	Internal_ApplyRuleOnTarget(RuleHolder,Target);

	/*apply the rule on the addional targets that are defined by the "Add" operator*/
	for (auto& itr : AdditionalTargets)
	{
		Internal_ApplyRuleOnTarget(RuleHolder, itr);
	}
		
	LastAppliedRule = RuleHolder;
}

void ATxT_RuleTarget::RemoveRuleFromTarget(ATxT_RuleHolder* RuleHolder)
{

	Internal_RemoveRuleFromTarget(RuleHolder, Target);

	for (auto& itr : AdditionalTargets)
	{
		Internal_RemoveRuleFromTarget(RuleHolder, itr);
	}

	LastAppliedRule = nullptr;
}

void ATxT_RuleTarget::Internal_ApplyRuleOnTarget(ATxT_RuleHolder* RuleHolder,TSubclassOf<ABaseBabaObject> target)
{
	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), target, FoundObjects);

	if (FoundObjects.Num() > 0 && !GM->bWasUndoMove)
	{
		//we take the first object and test for contradiction
		ABaseBabaObject* AsBabaObejct = Cast<ABaseBabaObject>(FoundObjects[0]);

		bool bRuleContradict = false;
		if(AsBabaObejct)
			bRuleContradict = RuleHolder->CheckForContradiction(AsBabaObejct);

		if (bRuleContradict)
		{
			ContradictionVisuals->SetVisibility(true);
			UE_LOG(LogTemp, Warning, TEXT("Rule Target : Rule Contradict"));
			return;
		}
	}

	bool ruleApplied = false;
	for (auto& itr : FoundObjects)
	{
		ABaseBabaObject* AsBabaObejct = Cast<ABaseBabaObject>(itr);
		if (AsBabaObejct)
		{
		
			if(AsBabaObejct->ApplyRuleOnObject(RuleHolder->Rule, RuleHolder->RuleID))
			{
				ruleApplied = true;
				if (GM)
				{
					if (RuleHolder->Rule->IsYouRule())
					{
						GM->IsYouObjects.AddUnique(AsBabaObejct);
					}
				}
			}
			else
			{
				break;//if the first one was not able to apply we are sure the rest wont since they are all instances of the same object.
			}
		}
	}

	if (GM && ruleApplied)
		GM->RegisterTargetForRule(RuleHolder->Rule, Target, RuleHolder->RuleID);
}

void ATxT_RuleTarget::Internal_RemoveRuleFromTarget(ATxT_RuleHolder* RuleHolder,TSubclassOf<ABaseBabaObject> target)
{
	if (!RuleHolder->Rule) return;

	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	ContradictionVisuals->SetVisibility(false);
	RuleHolder->ContradictionVisuals->SetVisibility(false);

	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), target, FoundObjects);

	for (auto& itr : FoundObjects)
	{
		ABaseBabaObject* AsBabaObejct = Cast<ABaseBabaObject>(itr);
		if (AsBabaObejct)
		{
			AsBabaObejct->RemoveRuleEffectFromObject(RuleHolder->Rule);

			if (GM)
			{
				if (RuleHolder->Rule->IsYouRule())
				{
					GM->IsYouObjects.Remove(AsBabaObejct);
					GM->OnYouObjectDied(AsBabaObejct);
				}
			}
		}
	}

	if (GM)
		GM->UnRegisterTargetFormRule(RuleHolder->Rule, target);
}

void ATxT_RuleTarget::TryApplyOnAdditionalTargets()
{
	if(LastAppliedRule)
	{
		for (auto& itr : AdditionalTargets)
		{
			Internal_ApplyRuleOnTarget(LastAppliedRule, itr);
		}
	}
}
