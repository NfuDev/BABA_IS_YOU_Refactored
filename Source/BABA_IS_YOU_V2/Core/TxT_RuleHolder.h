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
	/*Base Baba Text Object Interface*/
	virtual void TxTDoYourThing(EPushDirection ChangeDirection) override;
	/*Base Baba Text Object Interface*/


	UPROPERTY(EditAnywhere, Category = "Baba Core")
	UBabaRule* Rule;

	//proper place to check for contradiction is here not in the baba object level we want this to be only relevant to rules system not the baba level.
private:

	UPROPERTY()
	class ATxT_RuleActivator* LastAlingedActivatorUpper;

	UPROPERTY()
	class ATxT_RuleActivator* LastAlingedActivatorLeft;

};
