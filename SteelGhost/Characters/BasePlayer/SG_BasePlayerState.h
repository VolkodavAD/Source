// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SG_BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STEELGHOST_API ASG_BasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/** Whether to use motion controller location for aiming. */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Bogy")
		uint8 PlayerBodyState = 0;

	/* Header file of our PlayerState Child Class inside of the Class declaration */
	// Used to copy properties from the current PlayerState to the passed one
	virtual void CopyProperties(class APlayerState* PlayerState);
	// Used to override the current PlayerState with the properties of the passed one
	virtual void OverrideWith(class APlayerState* PlayerState);
};
