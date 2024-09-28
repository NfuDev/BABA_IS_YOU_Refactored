// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "../Gameplay/BabaRulesBase.h"
#include "BIY_GameMode.h"
#include "BaseBabaObject.Generated.h"


#if WITH_EDITOR

DECLARE_STATS_GROUP(TEXT("Baba Is You"), STATGROUP_BABA, STATCAT_Advanced);

DECLARE_CYCLE_STAT(TEXT("Visuals Update"), STAT_VisualsUpdate, STATGROUP_BABA);
DECLARE_CYCLE_STAT(TEXT("Post Location Changed"), STAT_PostLocationChanged, STATGROUP_BABA);

#define BABA_SCOPE_CYCLE_COUNTER(Scope) SCOPE_CYCLE_COUNTER(Scope);

#else

#define BABA_SCOPE_CYCLE_COUNTER(...)

#endif


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBabaEvent, AActor*, OverlappedObject);

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
	Live,
	Dead
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

	UFUNCTION(BlueprintCallable, Category = "Baba Functions")
	void ApplyRuleOnObject(UBabaRule* Rule);

	void SetTraceChannelResponce(ECollisionChannel Channel, ECollisionResponse Responce);//use instead of the set tracable

	bool Push(EPushDirection Direction);

	bool Effectcontradict(UBabaRuleEffect* Effect);

	/*Movement Inteface*/
	virtual void PreChangeLocation(){}
	virtual void PostChangeLocation(EPushDirection ChangeDirection) { CheckForOverlap(); }
	virtual void RegisterNeighbours() {};
	/*Movement Inteface*/


	void CheckForOverlap();

	UPROPERTY()
	EBabaObjectState BabaObjectState;

	void UpdateObjectState(EBabaObjectState NewState);

	/*mainly used for pushing objects , it detects by tracing against push channel*/
	ABaseBabaObject* GetObjectInGrid(EPushDirection Direction, FVector& outLoc);

	
	UPROPERTY()
	FBabaEvent OnBabaObjectOverlap;

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

	void UpdateVisuals();
	
	/*called on begin play to register and update visuals for the first time*/
	virtual void RegisterNeighbours() override;

	ABaseBabaObstacle* GetTileInDirection(EPushDirection Direction);

	UPROPERTY(EditAnywhere, Category = "Baba Core")
	TMap<EObstacleSegment, UPaperFlipbook*> VisualsCases;

	const TMap<FString, EObstacleSegment> stringToEnumMap;

	UPROPERTY()
	ABaseBabaObstacle* TopTile;

	UPROPERTY()
	ABaseBabaObstacle* BottomTile;

	UPROPERTY()
	ABaseBabaObstacle* RightTile;

	UPROPERTY()
	ABaseBabaObstacle* LeftTile;
};