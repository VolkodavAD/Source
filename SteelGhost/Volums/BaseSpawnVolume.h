// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Storage/Interactiv/SG_BasePickup.h"
#include "Components/BoxComponent.h"
#include "BaseSpawnVolume.generated.h"

UCLASS()
class STEELGHOST_API ABaseSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**Returns the WhereToSpawn subobject */
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	/** Find a random point within the BoxComponent */
	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetRandomPointInVolume();

	/** сколько создано элементов */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		int32 CurrentCountSpawnItem;
	/** сколько максимум может быть элементов */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		int32 MaxCountSpawnItem;

	void DicrimentCountItem();

protected:
	/** The pickup to spawn*/
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ASG_BasePickup> WhatToSpawn;

	FTimerHandle SpawnTimer;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float SpawnDelaySelect = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float SpawnDelayCurrent = 0.0f;

	/** Minimum spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnDelayRangeLow;
	/** Maximum spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnDelayRangeHigh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float LineTraceDistance;

private:
	/** Box Component to specify where pickups should be spawned */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WhereToSpawn; // WhereToSpawn;

		/** Handle spawning a new pickup */
	void SpawnPickup();

	/** The current spawn delay */
	float SpawnDelay;
};
