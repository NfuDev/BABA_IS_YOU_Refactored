// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "Effect_IsYou.h"
#include "../..//Core/BaseBabaObject.h"
#include "../../Core/BabaPlayerController.h"
#include "../..//Core/BIY_GameMode.h"

void UEffect_IsYou::AffectStarted()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (PC && GM)
	{
		ABabaPlayerController* AsbabaPC = Cast<ABabaPlayerController>(PC);
		if (AsbabaPC)
		{
			AsbabaPC->BindIsYouEffectInputSystem(this);
			UE_LOG(LogTemp, Warning, TEXT("Input Binded For Object %s"), *AffectedObject->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BABA DEBUG : Player Controller set in the game mode is not the right type"));
		}
		/*if (!InputComponent)
		{
			static const FName InputCompName(FString("BabaController_" + AffectedObject->GetName()));
		    InputComponent = NewObject<UInputComponent>(this, InputCompName);
		    InputComponent->bBlockInput = false;
		    InputComponent->Priority = GM->IsYouCount();

			if (UInputDelegateBinding::SupportsInputDelegate(GetClass()))
				UBIY_BindingUtil::BindInputDelegatesToObject(GetClass(), InputComponent, this);

			InputComponent->BindAction("Right", IE_Pressed,this, &UEffect_IsYou::MoveRight);
			InputComponent->BindAction("Left", IE_Pressed, this, &UEffect_IsYou::MoveLeft);
			InputComponent->BindAction("Up", IE_Pressed, this, &UEffect_IsYou::MoveUP);
			InputComponent->BindAction("Down", IE_Pressed, this, &UEffect_IsYou::MoveDown);
		}
		AffectedObject->bBlockInput = false;
		PC->PushInputComponent(InputComponent);*/
		GM->RegisterYouObject(AffectedObject);
		FVector NewLoc = AffectedObject->GetActorLocation();
		NewLoc.Z = 0.1f;
		AffectedObject->SetActorLocation(NewLoc);
	}
}

void UEffect_IsYou::EffectCleanup()
{
	//should check if there are more than one "is you" applied so we don't set it to zero and ruin the "is you" location
	FVector NewLoc = AffectedObject->GetActorLocation();
	NewLoc.Z = 0.0f;
	AffectedObject->SetActorLocation(NewLoc);
}

void UEffect_IsYou::MoveRight()
{
	if (!AffectedObject) return;
	if (!(AffectedObject->BabaObjectState == EBabaObjectState::ALive)) return;

	UE_LOG(LogTemp, Warning, TEXT("Called : ") __FUNCTION__);

	FVector NextGrid;
	ABaseBabaObject* NextTile = AffectedObject->GetObjectInGrid(EPushDirection::Right, NextGrid);

	if (!NextTile)
	{
		AffectedObject->SetActorLocation(NextGrid);
		AffectedObject->CheckForOverlap();//if we got here it means there is no object in that grid or there is a none pushable or none stop object
		AffectedObject->bBabaObjectUpdated = true;
	}
	else
	{
		if (NextTile->Push(EPushDirection::Right))
		{
			AffectedObject->SetActorLocation(NextGrid);
			AffectedObject->bBabaObjectUpdated = true;
		}
	}

}

void UEffect_IsYou::MoveLeft()
{
	if (!AffectedObject) return;
	if (!(AffectedObject->BabaObjectState == EBabaObjectState::ALive)) return;

	UE_LOG(LogTemp, Warning, TEXT("Called : ") __FUNCTION__);
	FVector NextGrid;
	ABaseBabaObject* NextTile = AffectedObject->GetObjectInGrid(EPushDirection::Left, NextGrid);

	if (!NextTile)
	{
		AffectedObject->SetActorLocation(NextGrid);
		AffectedObject->CheckForOverlap();
		AffectedObject->bBabaObjectUpdated = true;
	}
	else
	{
		if (NextTile->Push(EPushDirection::Left))
		{
			AffectedObject->SetActorLocation(NextGrid);
			AffectedObject->bBabaObjectUpdated = true;
		}
	}
}

void UEffect_IsYou::MoveUP()
{
	if (!AffectedObject) return;
	if (!(AffectedObject->BabaObjectState == EBabaObjectState::ALive)) return;

	UE_LOG(LogTemp, Warning, TEXT("Called : ") __FUNCTION__);
	FVector NextGrid;
	ABaseBabaObject* NextTile = AffectedObject->GetObjectInGrid(EPushDirection::UP, NextGrid);

	if (!NextTile)
	{
		AffectedObject->SetActorLocation(NextGrid);
		AffectedObject->CheckForOverlap();
		AffectedObject->bBabaObjectUpdated = true;
	}
	else
	{
		if (NextTile->Push(EPushDirection::UP))
		{
			AffectedObject->SetActorLocation(NextGrid);
			AffectedObject->bBabaObjectUpdated = true;
		}
	}
}

void UEffect_IsYou::MoveDown()
{
	if (!AffectedObject) return;
	if (!(AffectedObject->BabaObjectState == EBabaObjectState::ALive)) return;

	UE_LOG(LogTemp, Warning, TEXT("Called : ") __FUNCTION__);
	FVector NextGrid;
	ABaseBabaObject* NextTile = AffectedObject->GetObjectInGrid(EPushDirection::Down, NextGrid);

	if (!NextTile)
	{
		AffectedObject->SetActorLocation(NextGrid);
		AffectedObject->CheckForOverlap();
		AffectedObject->bBabaObjectUpdated = true;
	}
	else
	{
		if (NextTile->Push(EPushDirection::Down))
		{
			AffectedObject->SetActorLocation(NextGrid);
			AffectedObject->bBabaObjectUpdated = true;
		}
	}


}
