// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "../Gameplay/BabaRulesBase.h"
#include "BaseBabaObject.Generated.h"

#define Move(Args) std::move(Args)

#if WITH_EDITOR

DECLARE_STATS_GROUP(TEXT("Baba Is You"), STATGROUP_BABA, STATCAT_Advanced);

DECLARE_CYCLE_STAT(TEXT("Visuals Update"), STAT_VisualsUpdate, STATGROUP_BABA);
DECLARE_CYCLE_STAT(TEXT("Post Location Changed"), STAT_PostLocationChanged, STATGROUP_BABA);

#define BABA_SCOPE_CYCLE_COUNTER(Scope) SCOPE_CYCLE_COUNTER(Scope);

#else

#define BABA_SCOPE_CYCLE_COUNTER(...)

#endif


DECLARE_MULTICAST_DELEGATE_OneParam(FBabaEvent, AActor*);

UENUM(BlueprintType)
enum class EPushDirection : uint8
{
	UP,
	Down,
	Right,
	Left
};

UENUM(BlueprintType)
enum class EBabaObjectState : uint8
{
	ALive,
	Dead
};

// UNDO MECHANICS STUFF


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

	//FBabaObjectState(const FBabaObjectState& other)
	//{
	//	ObjectLocation = other.ObjectLocation;
	//	ObjectVisual = other.ObjectVisual;
	//	TopTile = other.TopTile;
	//	BottomTile = other.BottomTile;
	//	RightTile = other.RightTile;
	//	LeftTile = other.LeftTile;
	//	LastSavedVisualsID = other.LastSavedVisualsID;
	//	bIsValidState = other.bIsValidState;
	//	StateInstances = other.StateInstances;
	//}


	void UpdateStructWithBaba(ABaseBabaObject* BabaObject);
	void UpdateBabaWithStruct(ABaseBabaObject* BabaObject);
	
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

	UPROPERTY()
	EBabaObjectState BabaObjectState;

};



USTRUCT(BlueprintType, Blueprintable)
struct FBabaRuleWrapper
{
	GENERATED_BODY()

	FBabaRuleWrapper() {};
	FBabaRuleWrapper(UBabaRule* Rule, FGuid ID) : WrappedRule(Rule), RuleID(ID) {};

	UPROPERTY()
	UBabaRule* WrappedRule;

	UPROPERTY()
	FGuid RuleID;

	bool operator==(const FGuid& ID) const { return RuleID == ID; }
	bool operator==(const UBabaRule* Rule) const { return WrappedRule == Rule; }
};

/**
 * Base Baba Object , can be baba, Wall, Rule , etc
 */
UCLASS(Blueprintable,BlueprintType, HideDropDown)
class BABA_IS_YOU_V2_API ABaseBabaObject : public AActor
{

	GENERATED_BODY()
public:
	ABaseBabaObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabaIsYou")
	TObjectPtr<UPaperFlipbookComponent> VisualsComponent;

	virtual void BeginPlay() override;

	//exposed for testing only
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BabaIsYou")
	TArray<UBabaRuleEffect*> AppliedEffects;

	/*mapped one to one with the effects so we can easily remove the effects wihtout searching*/
	UPROPERTY()
	TArray<FBabaRuleWrapper> AppliedRules;

	UFUNCTION(BlueprintCallable, Category = "Baba Functions")
	bool ApplyRuleOnObject(UBabaRule* Rule, FGuid RuleID);

	bool CanApplyRule(FGuid RuleID);

	UFUNCTION(BlueprintCallable, Category = "Baba Functions")
	void RemoveRuleEffectFromObject(UBabaRule* Rule);

	UFUNCTION(BlueprintCallable, Category = "Baba Functions")
	FORCEINLINE int32 GetAppliedEffectCount() { return AppliedEffects.Num(); }

	void SetTraceChannelResponce(ECollisionChannel Channel, ECollisionResponse Responce);//use instead of the set tracable

	bool Push(EPushDirection Direction);

	bool Effectcontradict(UBabaRuleEffect* Effect);//from a desing prespective i think i should move this one layer up in the chain , so the txt objects take care of this

	/*Movement Inteface*/
	virtual void PreChangeLocation(){}
	virtual void PostChangeLocation(EPushDirection ChangeDirection) { CheckForOverlap(); }
	virtual void RegisterNeighbours() {};
	virtual void PostUndo() {};
	virtual void PostDeath() {};
	/*Movement Inteface*/


	void CheckForOverlap();


	UPROPERTY()
	EBabaObjectState BabaObjectState;

	void UpdateObjectState(EBabaObjectState NewState);

	/*mainly used for pushing objects , it detects by tracing against push channel*/
	ABaseBabaObject* GetObjectInGrid(EPushDirection Direction, FVector& outLoc);

	
	FBabaEvent OnBabaObjectOverlap;

	//to distinguish the objects that has changed and those hasn't
	bool bBabaObjectUpdated;

	/*for undo*/
	UPROPERTY()
	TArray<FBabaObjectState> ObjectStates;

	/*caching the last state before we go to next grid*/
	FBabaObjectState CachedObjectState;
	void RecordBabaObjectState();
	void OnBabaUndo();

private:
	
	bool CanBePushed();
	
};


UENUM(BlueprintType)
enum class EObstacleSegment : uint8
{
	// F = Grid is full
	// E = grid is empty
	//order = Top,Donw,Right,Left
	EEEE UMETA(DisplayName = "NoSides"),
	EEEF UMETA(DisplayName = "LeftOnly"),
	FEEE UMETA(DisplayName = "TopOnly"),
	FEEF UMETA(DisplayName = "TopAndLeft"),
	EEFE UMETA(DisplayName = "RightOnly"),
	EEFF UMETA(DisplayName = "RightAndLeft"),
	FEFE UMETA(DisplayName = "TopAndRight"),
	FEFF UMETA(DisplayName = "TopLeftAndRight"),
	EFEE UMETA(DisplayName = "DownOnly"),
	EFEF UMETA(DisplayName = "DownAndLeft"),
	FFEE UMETA(DisplayName = "TopAndDown"),
	FFEF UMETA(DisplayName = "TopDownAndLeft"),
	EFFE UMETA(DisplayName = "DownAndRight"),
	EFFF UMETA(DisplayName = "DownLeftAndRight"),
	FFFE UMETA(DisplayName = "TopDownAndRight"),
	FFFF UMETA(DisplayName = "AllSides")
};


/*
* this class represents all walls types in the game or obstacles
*/
UCLASS(HideDropDown)
class BABA_IS_YOU_V2_API ABaseBabaObstacle : public ABaseBabaObject
{
	GENERATED_BODY()

public:
	ABaseBabaObstacle();

	virtual void PostChangeLocation(EPushDirection ChangeDirection) override;
	virtual void PostDeath() override;

	void UpdateVisuals();
	
	/*called on begin play to register and update visuals for the first time*/
	virtual void RegisterNeighbours() override;

	ABaseBabaObstacle* GetTileInDirection(EPushDirection Direction);

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TMap<EObstacleSegment, UPaperFlipbook*> VisualsCases;

	const TMap<FString, EObstacleSegment> stringToEnumMap;

	UPROPERTY()
	FString LastSavedVisualsID;

	UPROPERTY()
	ABaseBabaObstacle* TopTile;

	UPROPERTY()
	ABaseBabaObstacle* BottomTile;

	UPROPERTY()
	ABaseBabaObstacle* RightTile;

	UPROPERTY()
	ABaseBabaObstacle* LeftTile;
};
