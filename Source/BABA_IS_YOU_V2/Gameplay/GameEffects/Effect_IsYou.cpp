// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "Effect_IsYou.h"
#include "../..//Core/BaseBabaObject.h"

void UEffect_IsYou::AffectStarted()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{

		if (!InputComponent)
		{
			static const FName InputCompName("BabaController");
		    InputComponent = NewObject<UInputComponent>(this, InputCompName);
		    InputComponent->bBlockInput = false;
		    InputComponent->Priority = 5;

			if (UInputDelegateBinding::SupportsInputDelegate(GetClass()))
				UBIY_BindingUtil::BindInputDelegatesToObject(GetClass(), InputComponent, this);

			InputComponent->BindAction("Right", IE_Pressed,this, &UEffect_IsYou::MoveRight);
			InputComponent->BindAction("Left", IE_Pressed, this, &UEffect_IsYou::MoveLeft);
			InputComponent->BindAction("Up", IE_Pressed, this, &UEffect_IsYou::MoveUP);
			InputComponent->BindAction("Down", IE_Pressed, this, &UEffect_IsYou::MoveDown);
		}

		PC->PushInputComponent(InputComponent);
	}
}

void UEffect_IsYou::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Called : ") __FUNCTION__);

	FVector NextGrid;
	ABaseBabaObject* NextTile = AffectedObject->GetObjectInGrid(EPushDirection::Right, NextGrid);

	if (!NextTile)
	{
		AffectedObject->SetActorLocation(NextGrid);
		AffectedObject->CheckForOverlap();//if we got here it means there is no object in that grid or there is a none pushable or none stop object
	}
	else
	{
		if (NextTile->Push(EPushDirection::Right))
		{
			AffectedObject->SetActorLocation(NextGrid);
		}
	}

}

void UEffect_IsYou::MoveLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Called : ") __FUNCTION__);
	FVector NextGrid;
	ABaseBabaObject* NextTile = AffectedObject->GetObjectInGrid(EPushDirection::Left, NextGrid);

	if (!NextTile)
	{
		AffectedObject->SetActorLocation(NextGrid);
		AffectedObject->CheckForOverlap();
	}
	else
	{
		if (NextTile->Push(EPushDirection::Left))
		{
			AffectedObject->SetActorLocation(NextGrid);
		}
	}
}

void UEffect_IsYou::MoveUP()
{
	UE_LOG(LogTemp, Warning, TEXT("Called : ") __FUNCTION__);
	FVector NextGrid;
	ABaseBabaObject* NextTile = AffectedObject->GetObjectInGrid(EPushDirection::UP, NextGrid);

	if (!NextTile)
	{
		AffectedObject->SetActorLocation(NextGrid);
		AffectedObject->CheckForOverlap();
	}
	else
	{
		if (NextTile->Push(EPushDirection::UP))
		{
			AffectedObject->SetActorLocation(NextGrid);
		}
	}
}

void UEffect_IsYou::MoveDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Called : ") __FUNCTION__);
	FVector NextGrid;
	ABaseBabaObject* NextTile = AffectedObject->GetObjectInGrid(EPushDirection::Down, NextGrid);

	if (!NextTile)
	{
		AffectedObject->SetActorLocation(NextGrid);
		AffectedObject->CheckForOverlap();
	}
	else
	{
		if (NextTile->Push(EPushDirection::Down))
		{
			AffectedObject->SetActorLocation(NextGrid);
		}
	}

	
}
