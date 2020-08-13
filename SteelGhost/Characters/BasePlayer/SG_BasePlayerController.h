// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SteelGhostCharacter.h"
#include "SG_BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STEELGHOST_API ASG_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//Properties
	UPROPERTY(VisibleAnywhere, Category = CustomProperties)
	class ASteelGhostCharacter * MyCharacter;

	// Called every frame
	// Input functions
	bool  bSomeBool;
	void  SetSomeBool(bool  bNewSomeBool);
	UFUNCTION(reliable, server, WithValidation)
	void  ServerSetSomeBool(bool  bNewSomeBool);


	// осмотр вверх и в стороны
	void Turn(float Rate);
	void LookUp(float Rate);

	FVector CurrentVelocity;

protected:
	virtual void PlayerTick(float DeltaSeconds) override;

	//Overriding SetupInputComponent
	void SetupInputComponent() override;
	void OnPossess(APawn* aPawn) override;
};
