// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SteelGhostGameMode.h"
#include "SteelGhostHUD.h"
#include "SteelGhostCharacter.h"
#include "PlayerControllerBody.h"
#include "UObject/ConstructorHelpers.h"

ASteelGhostGameMode::ASteelGhostGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/STEELGHOST/CHATACTERS/PLAYERS/CHARACTERS/BP_SG_BaseCharacter"));

	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASteelGhostHUD::StaticClass();

	PlayerControllerClass = APlayerControllerBody::StaticClass();
}
/* CPP file of our GameMode Child Class */
bool ASteelGhostGameMode::ReadyToStartMatch_Implementation() {
	//Super::ReadyToStartMatch();
	return true; //MaxNumPlayers == NumPlayers;
}/* CPP file of our GameMode Child Class */
void ASteelGhostGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	PlayerControllerList.Add(NewPlayer);
}

void ASteelGhostGameMode::ConnectToServer()
{
}

void ASteelGhostGameMode::DeadPlayerCharacter(ASG_BaseCharacter* DeadCharacter)
{
	if (DeadCharacter->GetController() != nullptr)
	{
		DeadCharacter->GetController()->UnPossess();
	}
	//DI->UnPossess();
}