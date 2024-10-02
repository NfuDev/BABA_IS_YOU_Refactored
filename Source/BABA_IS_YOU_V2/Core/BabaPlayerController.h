// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Gameplay/GameEffects/Effect_IsYou.h"

#include "BabaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BABA_IS_YOU_V2_API ABabaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void BindIsYouEffectInputSystem(UEffect_IsYou* IsYouInstance);
};
