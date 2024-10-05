// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "BabaTextObjectBase.h"
#include "TxT_RuleTarget.generated.h"

/**
 * this class holds a type of object to be affected by the rule that the acticator has, since the rules are written from left to right and up to down.
 */
UCLASS(HideDropDown)
class BABA_IS_YOU_V2_API ATxT_RuleTarget : public ABabaTextObjectBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TSubclassOf<ABaseBabaObject> Target;

	/*Base Baba Text Object Interface*/
	virtual void TxTDoYourThing(EPushDirection ChangeDirection) override;
	/*Base Baba Text Object Interface*/

	//account for when using 'And' Operator
	UPROPERTY()
	TArray<TSubclassOf<ABaseBabaObject>> AdditionalTargets;

	void ApplyRuleOnTarget(class ATxT_RuleHolder* RuleHolder);
	void RemoveRuleFromTarget(ATxT_RuleHolder* RuleHolder);


private:

	UPROPERTY()
	class ATxT_RuleActivator* LastAlingedActivatorRight;

	UPROPERTY()
	class ATxT_RuleActivator* LastAlingedActivatorBottom;
};
