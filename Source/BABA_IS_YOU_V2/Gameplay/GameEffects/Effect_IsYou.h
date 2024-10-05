// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "../BabaRuleEffectBase.h"
#include "../../Utils/BIY_BindingUtil.h"
#include "Effect_IsYou.generated.h"

/**
 * this effect will give the affected object an input component to be controlled by the player
 */
UCLASS()
class BABA_IS_YOU_V2_API UEffect_IsYou : public UBabaRuleEffect
{
	GENERATED_BODY()
	
public:

	UEffect_IsYou() {};

	virtual void AffectStarted() override;
	
	void MoveRight();
	void MoveLeft();
	void MoveUP();
	void MoveDown();

	UPROPERTY()
	UInputComponent* InputComponent;

};
