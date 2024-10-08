// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "Effect_IsPush.h"
#include "../..//Core/BaseBabaObject.h"
#include "../..//Core/BIY_GameMode.h"

void UEffect_IsPush::AffectStarted()
{

	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GM) return;
	ECollisionChannel Channle = UEngineTypes::ConvertToCollisionChannel(GM->PushChannel);
	AffectedObject->SetTraceChannelResponce(Channle, ECR_Block);
}

void UEffect_IsPush::EffectCleanup()
{
	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GM) return;
	ECollisionChannel Channle = UEngineTypes::ConvertToCollisionChannel(GM->PushChannel);
	AffectedObject->SetTraceChannelResponce(Channle, ECR_Ignore);
}
