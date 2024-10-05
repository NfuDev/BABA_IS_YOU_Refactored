// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BabaTextObjectBase.h"
#include "../Gameplay/GameEffects/Effect_IsPush.h"

void ABabaTextObjectBase::BeginPlay()
{
	Super::BeginPlay();

	//all text objects are pushable
	UBabaRuleEffect* temp = NewObject<UBabaRuleEffect>(this, UEffect_IsPush::StaticClass());
	temp->RegisterTarget(this);
	AppliedEffects.Add(temp);
}

ABabaTextObjectBase::ABabaTextObjectBase()
{
	ContradictionVisuals = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ContradictionVisuals"));
	ContradictionVisuals->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	ContradictionVisuals->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	ContradictionVisuals->SetupAttachment(RootComponent);
	ContradictionVisuals->SetVisibility(false);
}

void ABabaTextObjectBase::PostUndo()
{
	ContradictionVisuals->SetVisibility(false);
	TxTDoYourThing(EPushDirection());
}
