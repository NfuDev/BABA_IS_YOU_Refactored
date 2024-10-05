// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "../BabaRuleEffectBase.h"
#include "Effect_IsPush.generated.h"

/**
 * this effect allows the affected object to responce to traces in the push channel that is defined in the baba game mode
 */
UCLASS()
class BABA_IS_YOU_V2_API UEffect_IsPush : public UBabaRuleEffect
{
	GENERATED_BODY()
	

	virtual void AffectStarted() override;
	virtual void EffectCleanup() override;
};
