// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "BabaTextObjectBase.h"
#include "TxT_RuleHolder.generated.h"

/**
 * this holds the refernce to one of the rules singltons and gives the rule to the activator when the are alinged.
 */
UCLASS(HideDropDown)
class BABA_IS_YOU_V2_API ATxT_RuleHolder : public ABabaTextObjectBase
{
	GENERATED_BODY()
	
public:
	
	ATxT_RuleHolder() { RuleID = FGuid(); };

	/*Base Baba Text Object Interface*/
	virtual void TxTDoYourThing(EPushDirection ChangeDirection) override;
	/*Base Baba Text Object Interface*/


	virtual void PostUndo() override {};//those do not need to do any action post undo the activator only should care about this 

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	UBabaRule* Rule;

	//proper place to check for contradiction is here not in the baba object level we want this to be only relevant to rules system not the baba level.
	//also contradiction is part of the rule data asset for convenience, since this is just a holder it should not have anything to do with what will contradict the rule. 

	[[nodiscard]] bool CheckForContradiction(ABaseBabaObject* IncomingTarget);

	UPROPERTY()
	class ATxT_RuleActivator* LastAlingedActivatorUpper;

	UPROPERTY()
	class ATxT_RuleActivator* LastAlingedActivatorLeft;

	UPROPERTY()
	FGuid RuleID;

};
