// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BabaRulesBase.h"
#include "..//Core/BaseBabaObject.h"


UBabaRuleEffect* UBabaRule::GetRuleEffect(UObject* Outter)
{
	return  NewObject<UBabaRuleEffect>(Outter, RuleEffect);
}
