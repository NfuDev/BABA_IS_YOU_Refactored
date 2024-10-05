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
	/*Base Baba Text Object Interface*/
	virtual void TxTDoYourThing(EPushDirection ChangeDirection) override;
	/*Base Baba Text Object Interface*/

private:
	UPROPERTY()
	ATxT_RuleTarget* UpperTarget;

	UPROPERTY()
	ATxT_RuleTarget* LeftTarget;



	UPROPERTY()
	ATxT_RuleHolder* BottomRule;

	UPROPERTY()
	ATxT_RuleHolder* RightRule;


	UPROPERTY()
	ATxT_RuleHolder* LastAppliedRuleToUpper;

	UPROPERTY()
	ATxT_RuleHolder* LastAppliedRuleToLeft;
};
