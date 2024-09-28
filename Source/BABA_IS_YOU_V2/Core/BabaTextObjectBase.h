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
};
