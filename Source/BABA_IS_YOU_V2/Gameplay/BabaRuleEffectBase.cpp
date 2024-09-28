// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BabaRuleEffectBase.h"
#include "..//Core/BaseBabaObject.h"

UWorld* UBabaRuleEffect::GetWorld() const
{
#if WITH_EDITOR
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return nullptr;
	}

#endif
	const UObject* Outer = GetOuter();
	if (Outer == nullptr)
	{
		return nullptr;
	}
	return Outer->GetWorld();
}

void UBabaRuleEffect::RegisterTarget(ABaseBabaObject* targetObject)
{
	AffectedObject = targetObject;
	targetObject->AppliedEffects.Add(this);
	AffectStarted();
}
