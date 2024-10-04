// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "TxT_RuleHolder.h"
#include "TxT_RuleActivator.h"

//rules only cares about left and top , since the rules are written from left to right and up to down.
void ATxT_RuleHolder::TxTDoYourThing(EPushDirection ChangeDirection)
{
	//tell last one we moved
	if (LastAlingedActivatorUpper)
		LastAlingedActivatorUpper->TxTDoYourThing(ChangeDirection);
	if (LastAlingedActivatorLeft)
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

	return bContradict;
}
