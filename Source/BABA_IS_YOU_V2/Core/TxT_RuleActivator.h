// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "BabaTextObjectBase.h"
#include "TxT_RuleTarget.h"
#include "TxT_RuleHolder.h"
#include "TxT_RuleActivator.generated.h"

/**
 * this activates certain rules on certain objects type according to what rule target and rule holder that are alinged with
 * in each direction
 */
UCLASS(HideDropDown)
class BABA_IS_YOU_V2_API ATxT_RuleActivator : public ABabaTextObjectBase
{
	GENERATED_BODY()
	ATxT_RuleActivator() {};

public:

	/*Base Baba Object Interface*/
	virtual void PostChangeLocation(EPushDirection ChangeDirection) override { /*TxTDoYourThing(ChangeDirection);*/ };//now we wait for the game mode to invoke this and not when ever we move
	virtual void PostUndo() override { TxTDoYourThing(EPushDirection::UP); TxTDoYourThing(EPushDirection::Right); };
	/*Base Baba Object Interface*/

	/*Base Baba Text Object Interface*/
	virtual void TxTDoYourThing(EPushDirection ChangeDirection) override;
	/*Base Baba Text Object Interface*/

private:
	UPROPERTY()
	ATxT_RuleTarget* UpperTarget;

	UPROPERTY()
	ATxT_RuleTarget* LeftTarget;


	UPROPERTY()
	ATxT_RuleTarget* VerticalTransformTarget;

	UPROPERTY()
	ATxT_RuleTarget* HorizentalTransformTarget;


	UPROPERTY()
	ATxT_RuleHolder* BottomRule;

	UPROPERTY()
	ATxT_RuleHolder* RightRule;


	UPROPERTY()
	ATxT_RuleHolder* LastAppliedRuleToUpper;

	UPROPERTY()
	ATxT_RuleHolder* LastAppliedRuleToLeft;


	/*type swtich , when a target is placed instead of a rule , like instead of WALL IS STOP (TARGET _ ACTIVATOR _ RULE) we placed WALL IS BABA (TARGET _ ACTIVATOR _ TARGET)
    * so in this case we turn the wall into baba object and so on, and we need to store the previus type so we can undo this move since it is distructive
    */
	void PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> This, TSubclassOf<ABaseBabaObject> ToThis, bool bBetweenUpDownGrids,bool ReverseTypeSwitch, ATxT_RuleTarget* TransformationTarget);
	void Internal_PreformObjectTypeSwitch(TSubclassOf<ABaseBabaObject> This, TSubclassOf<ABaseBabaObject> ToThis, bool bBetweenUpDownGrids, bool ReverseTypeSwitch, ATxT_RuleTarget* TransformationTarget);

	std::tuple<bool, ATxT_RuleTarget*> TryActivate(ATxT_RuleTarget* TargetGrid, ABaseBabaObject* AssumedRuleGrid,bool bHorizental, TFunction<void(ATxT_RuleHolder*, ATxT_RuleTarget*)> PostActivationEvent);

	UPROPERTY()
	TSubclassOf<ABaseBabaObject> LeftRight_SwitchedToType;

	UPROPERTY()
	TSubclassOf<ABaseBabaObject> UpDown_SwitchedToType;

	//debugging only.
	int32 CallsCount = 0;
};


/*
* operator to stack multiple rules in one activation.
*/
UCLASS(HideDropDown)
class BABA_IS_YOU_V2_API ATxT_AND_Activator : public ABabaTextObjectBase
{
	GENERATED_BODY()

public:
	/*Base Baba Text Object Interface*/
	virtual void TxTDoYourThing(EPushDirection ChangeDirection) override;
	/*Base Baba Text Object Interface*/

};