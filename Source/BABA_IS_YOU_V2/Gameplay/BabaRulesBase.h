// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BabaRuleEffectBase.h"
#include "BabaRulesBase.generated.h"

/*
* this class is a singleton object and will store the affected object by the defined effect in this class and it monitor the apply and remove effects 
* on the objects
*/
UCLASS(Blueprintable, BlueprintType)
class UBabaRule : public UDataAsset
{
	GENERATED_BODY()

	UBabaRule() {};

	UPROPERTY(EditAnywhere, Category = "Baba Rules")
	TSubclassOf<UBabaRuleEffect> RuleEffect;

public:
	/*effects that if they were present on the target we send warning and the rule will fail to apply*/
	UPROPERTY(EditAnywhere, Category = "Baba Rules")
	TArray<UBabaRule*> ContradictedRules;

	//effect that has one of these rule we force overlap with.
	UPROPERTY(EditAnywhere, Category = "Baba Rules")
	TArray<UBabaRule*> InteractableRules;

	bool CanInteractWith(const UBabaRule* OtherRule)
	{
		return InteractableRules.Contains(OtherRule);
	}

	/*
	* the rule should have a list of affected objects so we can call remove effect from object and pass the rule and pass the object reference 
	* and the rule will check for it in the list and remove the effect
	*/

	UBabaRuleEffect* GetRuleEffect(UObject* Outter);
	bool IsYouRule();
};