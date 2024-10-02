// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BabaPlayerController.h"
#include "BIY_GameMode.h"

void ABabaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;

	InputComponent->BindAction("Right", IE_Pressed, GM, &ABIY_GameMode::TryUpdateBabaGameState);
	InputComponent->BindAction("Left", IE_Pressed, GM, &ABIY_GameMode::TryUpdateBabaGameState);
	InputComponent->BindAction("Up", IE_Pressed, GM, &ABIY_GameMode::TryUpdateBabaGameState);
	InputComponent->BindAction("Down", IE_Pressed, GM, &ABIY_GameMode::TryUpdateBabaGameState);

	InputComponent->BindAction("Undo", IE_Pressed, GM, &ABIY_GameMode::UndoMove);
}

void ABabaPlayerController::BindIsYouEffectInputSystem(UEffect_IsYou* IsYouInstance)
{
	InputComponent->BindAction("Right", IE_Pressed, IsYouInstance, &UEffect_IsYou::MoveRight);
	InputComponent->BindAction("Left", IE_Pressed, IsYouInstance, &UEffect_IsYou::MoveLeft);
	InputComponent->BindAction("Up", IE_Pressed, IsYouInstance, &UEffect_IsYou::MoveUP);
	InputComponent->BindAction("Down", IE_Pressed, IsYouInstance, &UEffect_IsYou::MoveDown);
}
