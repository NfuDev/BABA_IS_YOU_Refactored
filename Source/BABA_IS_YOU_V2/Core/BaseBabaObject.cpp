// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.


#include "BaseBabaObject.h"



ABaseBabaObject::ABaseBabaObject()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	VisualsComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Visuals Component"));
	VisualsComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	VisualsComponent->SetupAttachment(RootComponent);

	SetTraceChannelResponce(ECC_Visibility, ECR_Block);
}

void ABaseBabaObject::BeginPlay()
{
	Super::BeginPlay();

	//to make every baba object in the same plane
	FVector initialLocation = GetActorLocation();
	initialLocation.Z = 0.0f;
	SetActorLocation(initialLocation);
}

void ABaseBabaObject::ApplyRuleOnObject(UBabaRule* Rule)
{
	UBabaRuleEffect* temp = Rule->GetRuleEffect(this);
	temp->RegisterTarget(this);
	AppliedEffects.Add(temp);
}

void ABaseBabaObject::SetTraceChannelResponce(ECollisionChannel Channel, ECollisionResponse Responce)
{
	VisualsComponent->SetCollisionResponseToChannel(Channel, Responce);
}

bool ABaseBabaObject::Push(EPushDirection Direction)
{
	if (!CanBePushed()) return false;//this means we have an effect that prevents the pushing, other wise it will be false if blocked

	FVector NextGrid;
	ABaseBabaObject* NextTile = GetObjectInGrid(Direction,NextGrid);
	if (!NextTile)
	{
		PreChangeLocation();
		SetActorLocation(NextGrid);
		PostChangeLocation(Direction);
		return true;
	}
	else
	{
		if (NextTile->Push(Direction))
		{
			PreChangeLocation();
			SetActorLocation(NextGrid);
			PostChangeLocation(Direction);
			return true;
		}
		else
		{
			return false;
		}
	}
}


bool ABaseBabaObject::CanBePushed()
{
	bool bCanBePushed = true;
	for (auto& effect : AppliedEffects)
	{
		if (!effect->IsPushable())
		{
			bCanBePushed = false;
		}
	}

	return bCanBePushed;
}

bool ABaseBabaObject::Effectcontradict(UBabaRuleEffect* Effect)
{
	return false;// To DO
}

void ABaseBabaObject::CheckForOverlap()
{

	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GM) return;

	FHitResult GridCheckResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(), GM->TraceSize, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, GM->OverlapChecksDebugs, GridCheckResults, true, FColor::Red, FColor::Green);

	if (bHit)
	{
		//
	}
}

void ABaseBabaObject::UpdateObjectState(EBabaObjectState NewState)
{
	//To Do , logics for death animations and save that so when game rewind we can restore killed object
}

ABaseBabaObject* ABaseBabaObject::GetObjectInGrid(EPushDirection Direction, FVector& outLoc)
{
	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GM) return nullptr;
	FVector Dir;

	switch (Direction)
	{
	case EPushDirection::UP:
		Dir = GM->WorldForwardvector;
		break;
	case EPushDirection::Down:
		Dir = GM->WorldForwardvector * -1;
		break;
	case EPushDirection::Right:
		Dir = GM->WorldRightVector;
		break;
	case EPushDirection::Left:
		Dir = GM->WorldRightVector * -1;
		break;
	default:
		break;
	}
	FHitResult GridCheckResults;
	TArray<AActor*> ActorsToIgnore;

	outLoc = GetActorLocation() + (Dir * GM->GridSize);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), outLoc, outLoc, GM->TraceSize, GM->PushChannel, false, ActorsToIgnore, GM->PushChecksDebugs, GridCheckResults, true, FColor::Red, FColor::Green);

	if (bHit)
	{
		return Cast<ABaseBabaObject>(GridCheckResults.GetActor());
	}

	return nullptr;
}


//#############################################################################################
//#############################################################################################
//############### OBSTACLES DYNAMIC VISUALS CHANGE ############################################
//#############################################################################################
//#############################################################################################

ABaseBabaObstacle::ABaseBabaObstacle() : Super::ABaseBabaObject()
{
	VisualsCases.Add(EObstacleSegment::EEEE, nullptr);
	VisualsCases.Add(EObstacleSegment::EEEF, nullptr);
	VisualsCases.Add(EObstacleSegment::EEFE, nullptr);
	VisualsCases.Add(EObstacleSegment::EEFF, nullptr);
	VisualsCases.Add(EObstacleSegment::EFEE, nullptr);
	VisualsCases.Add(EObstacleSegment::EFEF, nullptr);
	VisualsCases.Add(EObstacleSegment::EFFE, nullptr);
	VisualsCases.Add(EObstacleSegment::EFFF, nullptr);
	VisualsCases.Add(EObstacleSegment::FEEE, nullptr);
	VisualsCases.Add(EObstacleSegment::FEEF, nullptr);
	VisualsCases.Add(EObstacleSegment::FEFE, nullptr);
	VisualsCases.Add(EObstacleSegment::FEFF, nullptr);
	VisualsCases.Add(EObstacleSegment::FFEE, nullptr);
	VisualsCases.Add(EObstacleSegment::FFEF, nullptr);
	VisualsCases.Add(EObstacleSegment::FFFE, nullptr);
	VisualsCases.Add(EObstacleSegment::FFFF, nullptr);

	
	const_cast<TMap<FString, EObstacleSegment>&>(stringToEnumMap) =
	{
		{"EEEE", EObstacleSegment::EEEE},
		{"EEEF", EObstacleSegment::EEEF},
		{"FEEE", EObstacleSegment::FEEE},
		{"FEEF", EObstacleSegment::FEEF},
		{"EEFE", EObstacleSegment::EEFE},
		{"EEFF", EObstacleSegment::EEFF},
		{"FEFE", EObstacleSegment::FEFE},
		{"FEFF", EObstacleSegment::FEFF},
		{"EFEE", EObstacleSegment::EFEE},
		{"EFEF", EObstacleSegment::EFEF},
		{"FFEE", EObstacleSegment::FFEE},
		{"FFEF", EObstacleSegment::FFEF},
		{"EFFE", EObstacleSegment::EFFE},
		{"EFFF", EObstacleSegment::EFFF},
		{"FFFE", EObstacleSegment::FFFE},
		{"FFFF", EObstacleSegment::FFFF}
	};

	VisualsComponent->SetCollisionObjectType(ECC_WorldStatic);
}

//to do : Optimise this ... first thing is to make it update on the changed diretion. [DONE]
void ABaseBabaObstacle::PostChangeLocation(EPushDirection ChangeDirection)
{
	Super::PostChangeLocation(ChangeDirection);

	BABA_SCOPE_CYCLE_COUNTER(STAT_PostLocationChanged);

	bool MovedTopDown = ChangeDirection == EPushDirection::UP || ChangeDirection == EPushDirection::Down;
	bool MovedLeftRight = ChangeDirection == EPushDirection::Left || ChangeDirection == EPushDirection::Right;

	//tell old neighbours am leaving
	if (TopTile && MovedLeftRight)
	{
		TopTile->BottomTile = nullptr;
		TopTile->UpdateVisuals();
	}
	if (BottomTile && MovedLeftRight)
	{
		BottomTile->TopTile = nullptr;
		BottomTile->UpdateVisuals();
	}
	if (RightTile && MovedTopDown)
	{
		RightTile->LeftTile = nullptr;
		RightTile->UpdateVisuals();
	}
	if (LeftTile && MovedTopDown)
	{
		LeftTile->RightTile = nullptr;
		LeftTile->UpdateVisuals();
	}

    //regester new neighbours
	if (MovedTopDown)
	{
		RightTile = GetTileInDirection(EPushDirection::Right);
		if (RightTile)
		{
			RightTile->LeftTile = this;
			RightTile->UpdateVisuals();
		}

		LeftTile = GetTileInDirection(EPushDirection::Left);
		if (LeftTile)
		{
			LeftTile->RightTile = this;
			LeftTile->UpdateVisuals();
		}

		if (ChangeDirection == EPushDirection::UP)
		{
			TopTile = GetTileInDirection(ChangeDirection);
			if (TopTile)
			{
				TopTile->BottomTile = this;
				TopTile->UpdateVisuals();
			}
		}
		else
		{
			BottomTile = GetTileInDirection(ChangeDirection);
			if (BottomTile)
			{
				BottomTile->TopTile = this;
				BottomTile->UpdateVisuals();
			}
		}
	}

	if (MovedLeftRight)
	{
		TopTile = GetTileInDirection(EPushDirection::UP);
		if (TopTile)
		{
			TopTile->BottomTile = this;
			TopTile->UpdateVisuals();
		}

		BottomTile = GetTileInDirection(EPushDirection::Down);
		if (BottomTile)
		{
			BottomTile->TopTile = this;
			BottomTile->UpdateVisuals();
		}

		if (ChangeDirection == EPushDirection::Right)
		{
			RightTile = GetTileInDirection(ChangeDirection);
			if (RightTile)
			{
				RightTile->LeftTile = this;
				RightTile->UpdateVisuals();
			}
		}
		else
		{
			LeftTile = GetTileInDirection(ChangeDirection);
			if (LeftTile)
			{
				LeftTile->RightTile = this;
				LeftTile->UpdateVisuals();
			}
		}
	}

	UpdateVisuals();
}

void ABaseBabaObstacle::UpdateVisuals()
{
	FString Result = IsValid(TopTile) ? "F" : "E";
	Result += IsValid(BottomTile) ? "F" : "E";
	Result += IsValid(RightTile) ? "F" : "E";
	Result += IsValid(LeftTile) ? "F" : "E";
	

	EObstacleSegment VisualResult = stringToEnumMap[Result];
	UPaperFlipbook* NewVisuals = VisualsCases[VisualResult];

	if (NewVisuals)
		VisualsComponent->SetFlipbook(NewVisuals);
}

void ABaseBabaObstacle::RegisterNeighbours()
{
	TopTile = GetTileInDirection(EPushDirection::UP);
	BottomTile = GetTileInDirection(EPushDirection::Down);
	RightTile = GetTileInDirection(EPushDirection::Right);
	LeftTile = GetTileInDirection(EPushDirection::Left);

	UpdateVisuals();
}

ABaseBabaObstacle* ABaseBabaObstacle::GetTileInDirection(EPushDirection Direction)
{
	ABIY_GameMode* GM = Cast< ABIY_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GM) return nullptr;
	FVector Dir;

	switch (Direction)
	{
	case EPushDirection::UP:
		Dir = GM->WorldForwardvector;
		break;
	case EPushDirection::Down:
		Dir = GM->WorldForwardvector * -1;
		break;
	case EPushDirection::Right:
		Dir = GM->WorldRightVector;
		break;
	case EPushDirection::Left:
		Dir = GM->WorldRightVector * -1;
		break;
	default:
		break;
	}
	
	TArray<AActor*> ActorsToIngore;
	FHitResult GridCheckResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
	Objects.Add(GM->ObstaclesType);
	FVector CheckLoc = GetActorLocation() + (Dir * GM->GridSize);

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), CheckLoc, CheckLoc, GM->TraceSize, Objects, false,
		 ActorsToIngore, GM->VisualsUpdateChecksDebugs, GridCheckResults, true, FColor::Red, FColor::Green);

	if (bHit)
	{
		return Cast<ABaseBabaObstacle>(GridCheckResults.GetActor());
	}

	return nullptr;
}