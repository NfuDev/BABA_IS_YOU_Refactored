// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "Baba_EffectsBatch.h"
#include "../..//Core/BaseBabaObject.h"
#include "Delegates/Delegate.h"
#include "Effect_IsYou.h"

//######## IS WIN #######################

void UEffect_IsWin::AffectStarted()
{
	AffectedObject->OnBabaObjectOverlap.AddUObject(this, &UEffect_IsWin::OnOverlap);
}

void UEffect_IsWin::OnOverlap(AActor* OverlappedObject)
{
	ABaseBabaObject* AsBabaObject = Cast<ABaseBabaObject>(OverlappedObject);
	if (AsBabaObject)
	{
		TArray<UBabaRuleEffect*> IsYouEffects;
		IsYouEffects = AsBabaObject->AppliedEffects.FilterByPredicate([](UBabaRuleEffect* itr)
			{
				return itr->IsA(UEffect_IsYou::StaticClass());
			});

		if (IsYouEffects.Num() > 0)
		{
			ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

			if (GM)
			{
				GM->BabaGameFinished(true);
			}

		}
	}
}

//######## IS WIN #######################

//######## IS KILL #######################

void UEffect_IsKill::AffectStarted()
{
	AffectedObject->OnBabaObjectOverlap.AddUObject(this, &UEffect_IsKill::OnOverlap);
}

void UEffect_IsKill::OnOverlap(AActor* OverlappedObject)
{
	ABaseBabaObject* AsBabaObject = Cast<ABaseBabaObject>(OverlappedObject);
	if (AsBabaObject)
	{
		TArray<UBabaRuleEffect*> IsYouEffects;
		IsYouEffects = AsBabaObject->AppliedEffects.FilterByPredicate([](UBabaRuleEffect* itr)
			{
				return itr->IsA(UEffect_IsYou::StaticClass());
			});

		if (IsYouEffects.Num() > 0)
		{
			ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

			if (GM)
			{
				GM->BabaGameFinished(true);
			}

		}
	}
}

//######## IS KILL #######################