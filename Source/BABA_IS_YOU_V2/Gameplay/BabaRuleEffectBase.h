// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "BabaRuleEffectBase.generated.h"

class ABaseBabaObject;
class UBabaRule;
/*
* this is the base effect class that has the main interface with the game logics
*/
UCLASS(Blueprintable, BlueprintType, HideDropDown)
class UBabaRuleEffect : public UObject
{
	GENERATED_BODY()

public:
	UBabaRuleEffect() {};

	UWorld* GetWorld() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Baba Is You")
	void ApplyEffect(ABaseBabaObject* targetObject);//probably should be called inside the affect started and pass the baba object so it can be made in blueprint also but this for later

	virtual void AffectStarted() {};
	virtual void EffectCleanup() {};

	UPROPERTY()
	ABaseBabaObject* AffectedObject;

	void RegisterTarget(ABaseBabaObject* targetObject);
	

	/*interactions*/
	virtual void PreOverlap(ABaseBabaObject* IncomingObject) {};
	virtual void OnOverlap(AActor* OverlappedObject) {};
	virtual void RequestMove(FVector Direction, TFunction<void()> Callback) { Callback; };
	virtual bool IsPushable() { return true; }
	virtual void UnRegisterTarget();//default behaviour is to nullify the affected target and mark as garbage so the GC would pick it up in the next cycle
	/*interactions*/
	
};