// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/InputDelegateBinding.h"
#include "Components/InputComponent.h"
#include "BIY_BindingUtil.generated.h"

/**
 * this is a blueprint library so we can add input components to new objects in the drived blueprints classes from the base baba object
 * but for now all the main objects call this directly from c++
 */
UCLASS()
class BABA_IS_YOU_V2_API UBIY_BindingUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//to bind our graph dynamic delegates to the player controller so they get fired too when the player presses an input
	static void BindInputDelegatesToObject(const UClass* InClass, UInputComponent* InputComponent, UObject* GraphObject);
};
