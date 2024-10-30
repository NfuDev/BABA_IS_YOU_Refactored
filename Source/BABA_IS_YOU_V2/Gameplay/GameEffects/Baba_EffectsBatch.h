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
	virtual bool IsPushable() override { return false; };
	virtual bool IsWining() override { return true; };
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

/*
* this effects will make it so when the owner overlaps with any type defined here it kills both of them.
* used like "is sink" or "is open" and so on , the overlap object and the owner will both get destroyed.
* exposed to blueprint since we want this to work with any effects in the game. defined in the editor
*/
UCLASS(Blueprintable, BlueprintType, Abstract)
class BABA_IS_YOU_V2_API UEffect_DistructiveInteraction : public UBabaRuleEffect
{
	GENERATED_BODY()

	/*the effect that should be in the other object so the distrcution happen*/
	UPROPERTY(EditAnywhere, Category = "Info")
	TSubclassOf<UBabaRuleEffect> OtherObjectEffect;

public:

	virtual void AffectStarted() override;

	virtual void OnOverlap(AActor* OverlappedObject) override;

};

/*
* this effect is just used to be checked for by the distructive interaction effect it doesn't require any logics or code
* the mere class presnce is what we need
*/
UCLASS(Blueprintable, BlueprintType, Abstract)
class BABA_IS_YOU_V2_API UEffect_DistructionType : public UBabaRuleEffect
{
	GENERATED_BODY()
};