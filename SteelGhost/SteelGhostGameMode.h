// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Characters/BasePlayer/SG_BasePlayerController.h"
#include "Characters/BasePlayer/SG_BaseCharacter.h"

#include "SteelGhostGameMode.generated.h"

UCLASS(minimalapi)
class ASteelGhostGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASteelGhostGameMode();

	void ConnectToServer();

protected:
	// Overriding the PostLogin function
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	/* Header file of our GameMode Child Class inside of the Class declaration */
	// Maximum Number of Players needed/allowed during this Match
	int32 MaxNumPlayers = 10;
	/* Header file of our GameMode Child Class inside of the Class declaration */
	// List of PlayerControllers
	TArray<class APlayerController*> PlayerControllerList;
	
	// Override Implementation of ReadyToStartMatch
	virtual bool ReadyToStartMatch_Implementation() override;
	
	virtual void DeadPlayerCharacter(ASG_BaseCharacter* ControllerContrller);
};