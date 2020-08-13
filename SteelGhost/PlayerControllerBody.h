// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SteelGhostCharacter.h"
#include "PlayerControllerBody.generated.h"

/**
 * 
 */
UCLASS()
class STEELGHOST_API APlayerControllerBody : public APlayerController
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

	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);

	// движение вперед/назад и всторону
	void MoveForward(float Val);
	void MoveRight(float Val);

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


