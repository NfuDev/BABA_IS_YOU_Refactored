// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "TxT_RuleTarget.h"
#include "Kismet/Gameplaystatics.h"

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
