// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_RaidGameState.h"

/* CPP file of our GameState Class */
// This function is required through the Replicated specifier in the UPROPERTY Macro and is declared by it
void ASG_RaidGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty > & OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASG_RaidGameState, TeamAScore);
	DOREPLIFETIME(ASG_RaidGameState, TeamBScore);
}
/* CPP file of our GameState Class */
void ASG_RaidGameState::AddScore(bool TeamAScored) {
	if (TeamAScored)
		TeamAScore++;
	else
		TeamBScore++;
}