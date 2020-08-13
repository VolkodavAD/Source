// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameState.h"
#include "Characters/BasePlayer/SG_BasePlayerState.h"
#include "Characters/BasePlayer/SG_BasePlayerController.h"
#include "SG_RaidGameState.generated.h"

/**
 * 
 */
UCLASS()
class STEELGHOST_API ASG_RaidGameState : public AGameState
{
	GENERATED_BODY()

public:
	// Replicated Specifier used to mark this variable to replicate
	UPROPERTY(Replicated)
	int32 TeamAScore;

	UPROPERTY(Replicated)
	int32 TeamBScore;
	// Function to increase the Score of a Team
	void AddScore(bool TeamAScored);
	
};
