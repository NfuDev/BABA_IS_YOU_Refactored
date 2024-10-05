// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "TxT_RuleActivator.h"


void ATxT_RuleActivator::TxTDoYourThing(EPushDirection ChangeDirection)
{
	if (UpperTarget && LastAppliedRuleToUpper)
		UpperTarget->RemoveRuleFromTarget(LastAppliedRuleToUpper);

	if (LeftTarget && LastAppliedRuleToLeft)
		LeftTarget->RemoveRuleFromTarget(LastAppliedRuleToLeft);

	FVector DummyVector = FVector();
	UpperTarget = Cast<ATxT_RuleTarget>(GetObjectInGrid(EPushDirection::UP, DummyVector));//since we only need the object and not wanting to move we dont need real vector
	BottomRule = Cast<ATxT_RuleHolder>(GetObjectInGrid(EPushDirection::Down, DummyVector));

	LeftTarget = Cast<ATxT_RuleTarget>(GetObjectInGrid(EPushDirection::Left, DummyVector));
	RightRule = Cast<ATxT_RuleHolder>(GetObjectInGrid(EPushDirection::Right, DummyVector));

	if (UpperTarget && BottomRule)
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

	if (LeftTarget && RightRule)
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
}
