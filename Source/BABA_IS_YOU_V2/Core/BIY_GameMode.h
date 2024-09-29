// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/Gameplaystatics.h"
#include "BIY_GameMode.generated.h"


class UBabaRuleEffect;
class UBabaRule;

/*holds two effects that contradict each other*/
USTRUCT(BlueprintType)
struct FContradictionList
{
	GENERATED_BODY()

	FContradictionList() {};

	UPROPERTY(EditAnywhere, Category = "Baba Structs")
	TSubclassOf<UBabaRuleEffect> Effect1;

	UPROPERTY(EditAnywhere, Category = "Baba Structs")
	TSubclassOf<UBabaRuleEffect> Effect2;

};

/*holds all the targets that the rule has applid to*/
USTRUCT(BlueprintType)
struct FRuleTargets
{
	GENERATED_BODY()

	FRuleTargets() {}

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Baba Structs")
	TArray<class ABaseBabaObject*> RuleTargets;

};

/**
 * has the game settings for more dynamic properties change instead of hardcoding everything in the c++ side
 */
UCLASS(Blueprintable, BlueprintType)
class BABA_IS_YOU_V2_API ABIY_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	ABIY_GameMode() {}

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Baba Core")
	FVector WorldForwardvector;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	FVector WorldRightVector;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	double GridSize;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	double TraceSize;

	/*List of effects that contradict each other*/
	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TArray<FContradictionList> ContraditctionList;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TEnumAsByte<EDrawDebugTrace::Type> GridChecksDebugs;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TEnumAsByte<EDrawDebugTrace::Type> ActivatorChecksDebugs;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TEnumAsByte<EDrawDebugTrace::Type> PushChecksDebugs;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TEnumAsByte<EDrawDebugTrace::Type> OverlapChecksDebugs;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TEnumAsByte<EDrawDebugTrace::Type> VisualsUpdateChecksDebugs;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TEnumAsByte<EObjectTypeQuery> CharactersType;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TEnumAsByte<EObjectTypeQuery> TextObjectType;

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TEnumAsByte<EObjectTypeQuery> ObstaclesType;

	/*if the object has this blocked then it is detectable for push, and the overlap by default will be in the visiblity channel*/
	UPROPERTY(EditAnywhere, Category = "Baba Core| Tracing")
	TEnumAsByte <ETraceTypeQuery> PushChannel;

	/*called when the game is finished*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Baba Gameplay")
	void BabaGameFinished(bool bWin);

	/*this will holds how many object is a 'YOU' and when this list is cleared the game is over*/
	UPROPERTY()
	TArray<class ABaseBabaObject*> IsYouObjects;
	void RegisterYouObject(ABaseBabaObject* newObject);
	void UnRegisterYouObject(ABaseBabaObject* Object);


	/*saves what rule affects which objects*/
	UPROPERTY()
	TMap<UBabaRule*, FRuleTargets> Rules_Targets_Map;
	void RegisterTargetForRule(UBabaRule* rule, ABaseBabaObject* target);
	void UnRegisterTargetFormRule(UBabaRule* rule, ABaseBabaObject* target);



	//###################################################
	//################# DEBUGGING #######################
	//###################################################

	UFUNCTION(BlueprintCallable, Category = "Baba Is You|Debugging")
	FORCEINLINE int32 IsYouCount() { return IsYouObjects.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Baba Is You|Debugging")
	FORCEINLINE TMap<UBabaRule*, FRuleTargets> GetRuleAndTargets() { return Rules_Targets_Map; };
};
