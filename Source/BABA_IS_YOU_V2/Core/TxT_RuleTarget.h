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

	/*type swtich , when a target is placed instead of a rule , like instead of WALL IS STOP (TARGET _ ACTIVATOR _ RULE) we placed WALL IS BABA (TARGET _ ACTIVATOR _ TARGET)
	* so in this case we turn the wall into baba object and so on, and we need to store the previus type so we can undo this move since it is distructive 
	*/
	void PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> NewType);

	UPROPERTY()
	TSubclassOf<ABaseBabaObject> PreviousType;

private:

	UPROPERTY()
	class ATxT_RuleActivator* LastAlingedActivatorRight;

	UPROPERTY()
	class ATxT_RuleActivator* LastAlingedActivatorBottom;
};
