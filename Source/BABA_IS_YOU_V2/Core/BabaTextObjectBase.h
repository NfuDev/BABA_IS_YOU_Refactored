// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "BaseBabaObject.h"
#include "BabaTextObjectBase.generated.h"

/**
 * this is the parent for the rules holders and targets and activators 
 * and it is pushable by defaults
 */
UCLASS(HideDropDown)
class BABA_IS_YOU_V2_API ABabaTextObjectBase : public ABaseBabaObject
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
public:

	ABabaTextObjectBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabaIsYou")
	TObjectPtr<UPaperFlipbookComponent> ContradictionVisuals;

	/*Base Baba Object Interface*/
	virtual void PostChangeLocation(EPushDirection ChangeDirection) override { TxTDoYourThing(ChangeDirection);};
	virtual void PostUndo() override;
	/*Base Baba Object Interface*/

	/*Base Baba Text Object Interface*/
	virtual void TxTDoYourThing(EPushDirection ChangeDirection) {};//override in dirved classed to do the txt behaviour accordingly
	/*Base Baba Text Object Interface*/
};
