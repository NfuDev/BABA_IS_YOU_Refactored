// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "../BabaRuleEffectBase.h"
#include "Baba_EffectsBatch.generated.h"

/**
 * ################ BATCH EFFECTS #####################
 * this file will have all the short and simple effects so i dont have to make a file per effect
 */


/*
* this effects give all the (Is You) objects a win if they overlapped with the affected object by this effect
*/
UCLASS()
class BABA_IS_YOU_V2_API UEffect_IsWin : public UBabaRuleEffect
{
	GENERATED_BODY()
	
	virtual void AffectStarted() override;

	virtual void OnOverlap(AActor* OverlappedObject) override;

};


/*
* this effects give all the (Is You) objects a Lose if they overlapped with the affected object by this effect
*/
UCLASS()
class BABA_IS_YOU_V2_API UEffect_IsKill : public UBabaRuleEffect
{
	GENERATED_BODY()

	virtual void AffectStarted() override;

	virtual void OnOverlap(AActor* OverlappedObject) override;

};