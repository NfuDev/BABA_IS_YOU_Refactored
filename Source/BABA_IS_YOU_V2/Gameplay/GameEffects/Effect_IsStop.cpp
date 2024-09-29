// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "Effect_IsStop.h"
#include "../..//Core/BaseBabaObject.h"

void UEffect_IsStop::AffectStarted()
{
	//tracable in the push channel but "IsPushable" override here returns false so it will prevent movement
	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GM) return;
	ECollisionChannel Channle = UEngineTypes::ConvertToCollisionChannel(GM->PushChannel);
	AffectedObject->SetTraceChannelResponce(Channle, ECR_Block);
}

//void UEffect_IsStop::UnRegisterTarget(ABaseBabaObject* targetObject)
//{
//
//	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//
//	if (!GM) return;
//	ECollisionChannel Channle = UEngineTypes::ConvertToCollisionChannel(GM->PushChannel);
//	AffectedObject->SetTraceChannelResponce(Channle, ECR_Ignore);
//
//	Super::UnRegisterTarget(targetObject);
//}
