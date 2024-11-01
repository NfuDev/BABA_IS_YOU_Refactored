// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/Gameplaystatics.h"
#include "BaseBabaObject.h"
#include "BIY_GameMode.generated.h"


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

	FRuleTargets() { bIsValid = false; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Baba Structs")
	TArray<UBabaRule*> AppliedRulesOnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Baba Structs")
	FGuid RuleID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Baba Structs")
	bool bIsValid;
};

/**
 * has the game settings for more dynamic properties change instead of hardcoding everything in the c++ side
 */
UCLASS(Blueprintable, BlueprintType)
class BABA_IS_YOU_V2_API ABIY_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	ABIY_GameMode();

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

	UPROPERTY()
	TEnumAsByte<EDrawDebugTrace::Type> Ignored;

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

	UFUNCTION(BlueprintCallable, Category = "Baba Gameplay")
	void InitBabaGame();

	/*this will holds how many object is a 'YOU' and when this list is cleared the game is over*/
	/*take into account if the 'YOU' is dead or alive because even if in the list but dead it should end the game*/
	UPROPERTY()
	TArray<ABaseBabaObject*> IsYouObjects;
	void RegisterYouObject(ABaseBabaObject* newObject);
	void UnRegisterYouObject(ABaseBabaObject* Object);
	void OnYouObjectDied(ABaseBabaObject* Object);
	void CheckGameFinished();//since we don't event use the object in this new code , i used new function with better name to call for when the game does some changes and want to check if the game has finished.

	/*saves what rule affects which objects*/
	UPROPERTY()
	TMap<TSubclassOf<ABaseBabaObject>, FRuleTargets> Map_RulesOnObjectType;
	void RegisterTargetForRule(UBabaRule* rule, TSubclassOf<ABaseBabaObject> ObjectType, FGuid RuleID);
	void UnRegisterTargetFormRule(UBabaRule* rule, TSubclassOf<ABaseBabaObject> ObjectType);
	FRuleTargets& GetRulesForObjectType(TSubclassOf<ABaseBabaObject> ObjectType);

	//save all the baba objects in the level once so we dont need to query them everytime we want to store the game state
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABaseBabaObject*> BabaObjectsInLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABaseBabaObject*> RuleActivatorsList;

	/*called after each change in the game state to store it for undo*/
	void TryUpdateBabaGameState();
	void UndoMove();
	void InitObjectsStates();
	void EvaluateAllRules();

	bool bWasUndoMove = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Baba Gameplay")
	void OnBabaUndo();

	//###################################################
	//################# DEBUGGING #######################
	//###################################################

	UFUNCTION(BlueprintCallable, Category = "Baba Is You|Debugging")
	FORCEINLINE int32 IsYouCount() { return IsYouObjects.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Baba Is You|Debugging")
	FORCEINLINE TMap<TSubclassOf<ABaseBabaObject>, FRuleTargets> GetRuleAndTargets() { return Map_RulesOnObjectType; };

	UFUNCTION(BlueprintImplementableEvent, Category = "Baba Is You|Debugging")
	void DebugPreviusLocation(FVector location, int entries, UPaperFlipbook* Visuals);

	UFUNCTION(BlueprintImplementableEvent, Category = "Baba Is You|Debugging")
	void DebugInputs();

};
