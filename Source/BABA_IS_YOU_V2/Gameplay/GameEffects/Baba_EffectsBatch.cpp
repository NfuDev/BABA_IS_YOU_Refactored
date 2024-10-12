// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "Baba_EffectsBatch.h"
#include "../..//Core/BaseBabaObject.h"
#include "../..//Core/BIY_GameMode.h"
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

//kill should kill not remove the effect from the target.
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
				AsBabaObject->UpdateObjectState(EBabaObjectState::Dead);
				GM->OnYouObjectDied(AsBabaObject);
			}

		}
	}
}

//######## IS KILL #######################

//######## Distructive Interaction ###################

void UEffect_DistructiveInteraction::AffectStarted()
{
	AffectedObject->OnBabaObjectOverlap.AddUObject(this, &UEffect_DistructiveInteraction::OnOverlap);
}

void UEffect_DistructiveInteraction::OnOverlap(AActor* OverlappedObject)
{
	ABaseBabaObject* AsBabaObject = Cast<ABaseBabaObject>(OverlappedObject);
	if (AsBabaObject)
	{
		TArray<UBabaRuleEffect*> IsYouEffects;
		IsYouEffects = AsBabaObject->AppliedEffects.FilterByPredicate([&](UBabaRuleEffect* itr)
			{
				return itr->IsA(OtherObjectEffect);
			});

		if (IsYouEffects.Num() > 0)
		{
			ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

			if (GM)
			{
				AsBabaObject->UpdateObjectState(EBabaObjectState::Dead);
				GM->OnYouObjectDied(AsBabaObject);

				AffectedObject->UpdateObjectState(EBabaObjectState::Dead);
				GM->OnYouObjectDied(AffectedObject);
			}

		}
	}

}
