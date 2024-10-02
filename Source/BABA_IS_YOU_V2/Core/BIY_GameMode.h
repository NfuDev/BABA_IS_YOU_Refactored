// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/Gameplaystatics.h"
#include "BaseBabaObject.h"
#include "PaperFlipbook.h"
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

USTRUCT(BlueprintType, Blueprintable)
struct FBabaObjectState
{
	GENERATED_BODY()


	FBabaObjectState() {};

	FBabaObjectState(ABaseBabaObject* BabaObject)
	{
		bIsValidState = true;
		UpdateStructWithBaba(BabaObject);
	}

	FBabaObjectState(const FBabaObjectState& other)
	{
		ObjectLocation = other.ObjectLocation;
		ObjectVisual = other.ObjectVisual;
		TopTile = other.TopTile;
		BottomTile = other.BottomTile;
		RightTile = other.RightTile;
		LeftTile = other.LeftTile;
		LastSavedVisualsID = other.LastSavedVisualsID;
		bIsValidState = other.bIsValidState;
	}

	void UpdateStructWithBaba(ABaseBabaObject* BabaObject)
	{
		if (!BabaObject) return;

		ObjectLocation = BabaObject->GetActorLocation();
		ObjectVisual = BabaObject->VisualsComponent->GetFlipbook();

		ABaseBabaObstacle* AsObstacle = Cast<ABaseBabaObstacle>(BabaObject);
		if (AsObstacle)
		{
			TopTile = AsObstacle->TopTile;
			BottomTile = AsObstacle->BottomTile;
			RightTile = AsObstacle->RightTile;
			LeftTile = AsObstacle->LeftTile;
			LastSavedVisualsID = AsObstacle->LastSavedVisualsID;
		}
	}

	void UpdateBabaWithStruct(ABaseBabaObject* BabaObject)
	{
		if (!BabaObject) return;

		BabaObject->SetActorLocation(ObjectLocation);
		BabaObject->VisualsComponent->SetFlipbook(ObjectVisual);
		BabaObject->bBabaObjectUpdated = false;

		ABaseBabaObstacle* AsObstacle = Cast<ABaseBabaObstacle>(BabaObject);
		if (AsObstacle)
		{
		   AsObstacle->TopTile = TopTile;
		   AsObstacle->BottomTile = BottomTile;
		   AsObstacle->RightTile = RightTile;
		   AsObstacle->LeftTile = LeftTile;
		   AsObstacle->LastSavedVisualsID = LastSavedVisualsID;
		}
	}
	//this holds the repeated moments so we increament this int instead of storing the whole thing again.
	int32 StateInstances;

	/*this to differ between the states that are made by actual objects and states that are returned from a function as a default return value 
	* when condition is not met
	*/
	UPROPERTY()
	bool bIsValidState;

	/*Base Baba Object Entries */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ObjectLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* ObjectVisual;

	/* Obstacles Entries */
	
	UPROPERTY()
	ABaseBabaObstacle* TopTile;

	UPROPERTY()
	ABaseBabaObstacle* BottomTile;

	UPROPERTY()
	ABaseBabaObstacle* RightTile;

	UPROPERTY()
	ABaseBabaObstacle* LeftTile;

	UPROPERTY()
	FString LastSavedVisualsID;

};

USTRUCT(BlueprintType, Blueprintable)
struct FBabaObjectStateWrapper
{
	GENERATED_BODY()
	FBabaObjectStateWrapper() {};

	FBabaObjectStateWrapper(ABaseBabaObject* BabaObject)
	{
		RecordBabaObject(BabaObject);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray< FBabaObjectState> Values;

	void RecordBabaObject(ABaseBabaObject* BabaObject)
	{
		int32 index = Values.Add(FBabaObjectState(BabaObject));
		
		UE_LOG(LogTemp, Warning, TEXT("BABA UNDO DEBUG : State Wrapper Constrcut state with visuals = %s"), *Values[index].ObjectVisual->GetName());
	}

	FBabaObjectState GetBabaObjectState()
	{
		FBabaObjectState State;

		//removing the last index since it is the state that the object is in before the undo so we dont need to undo to it 
		/*if (Values.IsValidIndex(Values.Num() - 1))
			Values.RemoveAt((Values.Num() - 1));*/

		int32 index = Values.Num() - 1;//this will become the second last index
	
		if (Values.IsValidIndex(index))
		{
			State = Values[index];
			Values.RemoveAt(index);
		}
	       
		
	    return State;
	}
};

/*holds all the targets that the rule has applid to*/
USTRUCT(BlueprintType)
struct FRuleTargets
{
	GENERATED_BODY()

	FRuleTargets() {}

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Baba Structs")
	TArray<ABaseBabaObject*> RuleTargets;

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
	virtual void Tick(float DeltaSeconds) override;

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
	TArray<ABaseBabaObject*> IsYouObjects;
	void RegisterYouObject(ABaseBabaObject* newObject);
	void UnRegisterYouObject(ABaseBabaObject* Object);


	/*saves what rule affects which objects*/
	UPROPERTY()
	TMap<UBabaRule*, FRuleTargets> Rules_Targets_Map;
	void RegisterTargetForRule(UBabaRule* rule, ABaseBabaObject* target);
	void UnRegisterTargetFormRule(UBabaRule* rule, ABaseBabaObject* target);


	//save all the baba objects in the level once so we dont need to query them everytime we want to store the game state
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ABaseBabaObject*, FBabaObjectStateWrapper> BabaObjectsStatesMap;

	/*called after each change in the game state to store it for undo*/
	void TryUpdateBabaGameState();
	void UndoMove();
	int32 MoveStacIndex;

	//###################################################
	//################# DEBUGGING #######################
	//###################################################

	UFUNCTION(BlueprintCallable, Category = "Baba Is You|Debugging")
	FORCEINLINE int32 IsYouCount() { return IsYouObjects.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Baba Is You|Debugging")
	FORCEINLINE TMap<UBabaRule*, FRuleTargets> GetRuleAndTargets() { return Rules_Targets_Map; };

	UFUNCTION(BlueprintImplementableEvent, Category = "Baba Is You|Debugging")
	void DebugPreviusLocation(FVector location, int entries, UPaperFlipbook* Visuals);

};
