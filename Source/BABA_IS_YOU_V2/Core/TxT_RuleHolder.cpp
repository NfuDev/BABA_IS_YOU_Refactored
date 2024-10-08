// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "TxT_RuleHolder.h"
#include "TxT_RuleActivator.h"


//rules only cares about left and top , since the rules are written from left to right and up to down.
void ATxT_RuleHolder::TxTDoYourThing(EPushDirection ChangeDirection)
{
	bool bIsVerticalMovement = (ChangeDirection == EPushDirection::UP || ChangeDirection == EPushDirection::Down);
	bool bIsHorizentalMovement = (ChangeDirection == EPushDirection::Left || ChangeDirection == EPushDirection::Right);
	//tell last one we moved
	if (LastAlingedActivatorUpper && bIsHorizentalMovement)
		LastAlingedActivatorUpper->TxTDoYourThing(ChangeDirection);
	if (LastAlingedActivatorLeft && bIsVerticalMovement)
		LastAlingedActivatorLeft->TxTDoYourThing(ChangeDirection);

	FVector DummyVector = FVector();
	  LastAlingedActivatorLeft = Cast<ATxT_RuleActivator>(GetObjectInGrid(EPushDirection::Left, DummyVector));

	  LastAlingedActivatorUpper = Cast<ATxT_RuleActivator>(GetObjectInGrid(EPushDirection::UP, DummyVector));

	//try trigger new activator if found
	if (LastAlingedActivatorUpper)
		LastAlingedActivatorUpper->TxTDoYourThing(ChangeDirection);
	if (LastAlingedActivatorLeft)
		LastAlingedActivatorLeft->TxTDoYourThing(ChangeDirection);
}

bool ATxT_RuleHolder::CheckForContradiction(ABaseBabaObject* IncomingTarget)
{
	if (!IncomingTarget || !Rule) return false;
	if (Rule->ContradictedRules.Num() == 0) return false;

	bool bContradict = false;

	for (auto& itr : IncomingTarget->AppliedRules)
	{
		bContradict = Rule->ContradictedRules.Contains(itr);
		if (bContradict)
			break;
	}

	//add visuals and game cues here for the contradiction
	if (bContradict)
		ContradictionVisuals->SetVisibility(true);

	return bContradict;
}
