// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BIY_GameMode.h"
#include "BaseBabaObject.h"

void ABIY_GameMode::BeginPlay()
{
	TArray<AActor*> BabaObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBabaObject::StaticClass(), BabaObjects);

	for (auto& babaObejct : BabaObjects)
	{
		ABaseBabaObject* AsBabaObject = Cast<ABaseBabaObject>(babaObejct);
		if (AsBabaObject)
		{
			AsBabaObject->RegisterNeighbours();
		}
	}
}
