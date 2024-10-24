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
	virtual void EffectCleanup() override;

	void MoveRight();
	void MoveLeft();
	void MoveUP();
	void MoveDown();

	//since the last move will happen always is the "Is YOu" object move then it is the best place to tell the game mode that all objects are updated so we evaluate new rules.
	void InvokeGameStateChanged();

	UPROPERTY()
	UInputComponent* InputComponent;

};
