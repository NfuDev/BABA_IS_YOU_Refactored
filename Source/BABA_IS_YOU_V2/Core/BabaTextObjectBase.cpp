// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BabaTextObjectBase.h"
#include "../Gameplay/GameEffects/Effect_IsPush.h"

void ABabaTextObjectBase::BeginPlay()
{
	Super::BeginPlay();

	//all text objects are pushable
	UBabaRuleEffect* temp = NewObject<UBabaRuleEffect>(this, UEffect_IsPush::StaticClass());
	temp->RegisterTarget(this);
	AppliedEffects.Add(temp);
}
