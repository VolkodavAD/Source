// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SG_BaseDeposit.generated.h"

UCLASS()
class STEELGHOST_API ASG_BaseDeposit : public AActor //, public ISG_ItemsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASG_BaseDeposit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceID")
		int32 ResourceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceID")
		int32 ResourceCount;
};
