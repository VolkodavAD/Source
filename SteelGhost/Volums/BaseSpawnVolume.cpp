// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpawnVolume.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABaseSpawnVolume::ABaseSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Box Component to represent the spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("ASD"), true);
	RootComponent = WhereToSpawn;

	//Set the spawn delay range
	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 4.5f;

	LineTraceDistance = 500.0f;
}

// Called when the game starts or when spawned
void ABaseSpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	//GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABaseSpawnVolume::SpawnPickup, SpawnDelay, false);
}

// Called every frame
void ABaseSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority)
	{
		if (CurrentCountSpawnItem < MaxCountSpawnItem)
		{
			if (SpawnDelayCurrent >= SpawnDelaySelect)
			{
				SpawnPickup();
				CurrentCountSpawnItem++;
				SpawnDelaySelect = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
				SpawnDelayCurrent = 0.0f;
			}
			else
			{
				SpawnDelayCurrent += DeltaTime;
			}
		}
	}
}


FVector ABaseSpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
	//return FVector::ZeroVector;
}



void ABaseSpawnVolume::SpawnPickup()
{
	if (Role == ROLE_Authority)
	{
		// If we have set something to spawn:
		if (WhatToSpawn != NULL)
		{
			// Check for a valid World: 
			UWorld* const World = GetWorld();
			if (World)
			{
				// Set the spawn parameters
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				// Get a random location to spawn at
				FVector SpawnLocation = GetRandomPointInVolume();

				// Get a random rotation for the spawned item
				FRotator SpawnRotation;
				SpawnRotation.Yaw = FMath::FRand() * 360.0f;
				SpawnRotation.Pitch = FMath::FRand() * 360.0f;
				SpawnRotation.Roll = FMath::FRand() * 360.0f;

				FVector Start;
				FVector End;

				Start = SpawnLocation;
				End = SpawnLocation - (GetActorUpVector() * LineTraceDistance);

				// additional trace parameters
				FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
				TraceParams.bTraceComplex = false;
				TraceParams.bReturnPhysicalMaterial = false;
				TraceParams.MobilityType = EQueryMobilityType::Static;
				//Re-initialize hit info
				FHitResult HitDetails = FHitResult(ForceInit);
				bool bIsHit = GetWorld()->LineTraceSingleByChannel(
					HitDetails,      // FHitResult object that will be populated with hit info
					Start,      // starting position
					End,        // end position
					ECC_GameTraceChannel3,  // collision channel - 3rd custom one
					TraceParams      // additional trace settings
				);

				DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 255), false, 1, 0, 1);

				if (bIsHit)
				{
					SpawnLocation = HitDetails.ImpactPoint;
					// spawn the pickup
					ASG_BasePickup* const SpawnedPickup = World->SpawnActor<ASG_BasePickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
					//ASG_BasePickup* const SpawnedPickup = World->SpawnActorDeferred(WhatToSpawn,SpawnLocation, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn)
					//ASG_BasePickup* const SpawnedPickup = World->SpawnActorDeferred(WhatToSpawn, SpawnLocation, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					SpawnedPickup->SetOwner(this);
					SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
				}

				//GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
			}
		}
	}

}
void ABaseSpawnVolume::DicrimentCountItem()
{
	CurrentCountSpawnItem--;
}

