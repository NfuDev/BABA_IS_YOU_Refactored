// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "TxT_RuleTarget.h"
#include "Kismet/Gameplaystatics.h"
#include "TxT_RuleActivator.h"

//targets cares only about the right and bottom grids
void ATxT_RuleTarget::TxTDoYourThing(EPushDirection ChangeDirection)
{
	if (LastAlingedActivatorRight)
		LastAlingedActivatorRight->TxTDoYourThing(ChangeDirection);

	if (LastAlingedActivatorBottom)
		LastAlingedActivatorBottom->TxTDoYourThing(ChangeDirection);

	FVector DummyVector = FVector();
	LastAlingedActivatorRight = Cast<ATxT_RuleActivator>(GetObjectInGrid(EPushDirection::Right, DummyVector));
	LastAlingedActivatorBottom = Cast<ATxT_RuleActivator>(GetObjectInGrid(EPushDirection::Down, DummyVector));
    

	if (LastAlingedActivatorRight)
		LastAlingedActivatorRight->TxTDoYourThing(ChangeDirection);

	if (LastAlingedActivatorBottom)
		LastAlingedActivatorBottom->TxTDoYourThing(ChangeDirection);
}

void ATxT_RuleTarget::ApplyRuleOnTarget(ATxT_RuleHolder* RuleHolder)
{
	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Target, FoundObjects);

	if (FoundObjects.Num() > 0)
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

	for (auto& itr : FoundObjects)
	{
		ABaseBabaObject* AsBabaObejct = Cast<ABaseBabaObject>(itr);
		if (AsBabaObejct)
		{
			AsBabaObejct->ApplyRuleOnObject(RuleHolder->Rule);
		}
	}
}

void ATxT_RuleTarget::RemoveRuleFromTarget(ATxT_RuleHolder* RuleHolder)
{
	if (!RuleHolder->Rule) return;

	ContradictionVisuals->SetVisibility(false);
	RuleHolder->ContradictionVisuals->SetVisibility(false);

	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Target, FoundObjects);

	for (auto& itr : FoundObjects)
	{
		ABaseBabaObject* AsBabaObejct = Cast<ABaseBabaObject>(itr);
		if (AsBabaObejct)
		{
			AsBabaObejct->RemoveRuleEffectFromObject(RuleHolder->Rule);
		}
	}
}
