// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_BasePlayerState.h"
#include "Net/UnrealNetwork.h"

void ASG_BasePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASG_BasePlayerState, PlayerBodyState);
	//DOREPLIFETIME(ASG_BaseCharacter, FirstPersonCameraComponent);
	//DOREPLIFETIME_CONDITION(ASG_BaseCharacter, CharacterStorage, COND_SkipOwner);
}

/* CPP file of our PlayerState Child Class */
void ASG_BasePlayerState::CopyProperties(class APlayerState* PlayerState) {
	Super::CopyProperties(PlayerState);
	if (PlayerState) {
		ASG_BasePlayerState* TestPlayerState = Cast<ASG_BasePlayerState>(PlayerState);
		if (TestPlayerState)
			TestPlayerState->PlayerBodyState = PlayerBodyState;
	}
}
void ASG_BasePlayerState::OverrideWith(class APlayerState* PlayerState) {
	Super::OverrideWith(PlayerState);
	if (PlayerState) {
		ASG_BasePlayerState* TestPlayerState = Cast<ASG_BasePlayerState>(PlayerState);
		if (TestPlayerState)
			PlayerBodyState = TestPlayerState->PlayerBodyState;
	}
}
/*
void ASG_BasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASG_BasePlayerState, PlayerBodyState);
}
*/