// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "../BabaRuleEffectBase.h"
#include "Effect_IsStop.generated.h"

/**
 * this class will also allow for trace responce but also blocks the push behaviour
 */
UCLASS()
class BABA_IS_YOU_V2_API UEffect_IsStop : public UBabaRuleEffect
{
	GENERATED_BODY()

	virtual void AffectStarted() override;
	virtual bool IsPushable() override { return false; };
	virtual void EffectCleanup() override;

	//virtual void UnRegisterTarget(ABaseBabaObject* targetObject) override;
};
