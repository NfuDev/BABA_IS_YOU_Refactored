// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/Gameplaystatics.h"
#include "BIY_GameMode.generated.h"


class UBabaRuleEffect;

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
};
