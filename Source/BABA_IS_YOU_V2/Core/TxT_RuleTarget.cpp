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

void ATxT_RuleTarget::ApplyRuleOnTarget(UBabaRule* Rule)
{
	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Target, FoundObjects);

	for (auto& itr : FoundObjects)
	{
		ABaseBabaObject* AsBabaObejct = Cast<ABaseBabaObject>(itr);
		if (AsBabaObejct)
		{
			AsBabaObejct->ApplyRuleOnObject(Rule);
		}
	}
}

void ATxT_RuleTarget::RemoveRuleFromTarget(UBabaRule* Rule)
{
	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Target, FoundObjects);

	for (auto& itr : FoundObjects)
	{
		ABaseBabaObject* AsBabaObejct = Cast<ABaseBabaObject>(itr);
		if (AsBabaObejct)
		{
			AsBabaObejct->RemoveRuleEffectFromObject(Rule);
		}
	}
}
