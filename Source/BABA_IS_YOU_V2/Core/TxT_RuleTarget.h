// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "BabaTextObjectBase.h"
#include "TxT_RuleTarget.generated.h"

/**
 * this class holds a type of object to be affected by the rule that the acticator has.
 */
UCLASS(HideDropDown)
class BABA_IS_YOU_V2_API ATxT_RuleTarget : public ABabaTextObjectBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TSubclassOf<ABaseBabaObject> Target;

	//account for when using 'And' Operator
	UPROPERTY()
	TArray<TSubclassOf<ABaseBabaObject>> AdditionalTargets;

	void ApplyRuleOnTarget(UBabaRule* Rule);
	void RemoveRuleFromTarget(UBabaRule* Rule);
};
