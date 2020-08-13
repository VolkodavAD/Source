// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBody.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

void APlayerControllerBody::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (ASteelGhostCharacter* MyPawn = Cast<ASteelGhostCharacter>(GetPawn()))
		{
			if (!CurrentVelocity.IsZero())
			{
				FVector NewLocation = MyPawn->GetActorLocation() + (CurrentVelocity * DeltaTime);
				MyPawn->SetActorLocation(NewLocation);
			}
		}
	}
}

void APlayerControllerBody::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);
}

void APlayerControllerBody::SetupInputComponent() {
	Super::SetupInputComponent();
	//this->InputComponent->BindAxis("MoveForward", this, &APlayerControllerBody::Move_XAxis);
	//this->InputComponent->BindAxis("MoveRight", this, &APlayerControllerBody::Move_YAxis);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//this->InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	this->InputComponent->BindAxis("Turn", this, &APlayerControllerBody::Turn);
	//this->InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	this->InputComponent->BindAxis("LookUp", this, &APlayerControllerBody::LookUp);
}

void APlayerControllerBody::Move_XAxis(float AxisValue)
{
	//APawn* Pawn = this->GetPawn();
	//CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
	if (this->GetPawn() && (AxisValue > 0.0f))
	{
		//ASteelGhostCharacter* OtherCharacter = Cast<ASteelGhostCharacter>(this->GetPawn());
		//FString NameCharacte = OtherCharacter->GetName();
		//UE_LOG(LogTemp, Log, TEXT("Actor is - %s"), *NameCharacte);

		//this->GetPawn()->AddMovementInput(this->GetPawn()->GetActorForwardVector(), AxisValue);
	}
}

void APlayerControllerBody::Move_YAxis(float AxisValue)
{
	//APawn* Pawn = this->GetPawn();
	//CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
}

void APlayerControllerBody::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		//AddMovementInput(GetActorForwardVector(), Value);
	}
}
void APlayerControllerBody::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		//AddMovementInput(GetActorRightVector(), Value);
		
	}
}

void APlayerControllerBody::Turn(float Rate)
{
	float BaseTurnRate = 1.0f;
	FRotator NewRotation = this->GetControlRotation() + FRotator(0.0f, 1.0f, 0.0f) * (Rate * BaseTurnRate);
	this->SetControlRotation(NewRotation);

	FString s = "ss";
	//UE_LOG(LogTemp, Log, TEXT("%s Rate = %f"), *NewRotation.ToString(), Rate);

	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void APlayerControllerBody::LookUp(float Rate)
{
	float BaseLookUpRate = -1.0f;
	FRotator NewRotation = this->GetControlRotation() + FRotator(1.0f, 0.0f, 0.0f) * (Rate * BaseLookUpRate);
	this->SetControlRotation(NewRotation);
	// calculate delta for this frame from the rate information
	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerControllerBody::SetSomeBool(bool bNewSomeBool)
{
	// Change the value of the bSomeBool property
	bSomeBool = bNewSomeBool;

	// If this next check succeeds, we are *not* the authority, meaning we are a network client.
	// In this case we also want to call the server function to tell it to change the bSomeBool property as well.
	if (Role < ROLE_Authority)
	{
		ServerSetSomeBool(bNewSomeBool);
	}
}

bool APlayerControllerBody::ServerSetSomeBool_Validate(bool bNewSomeBool)
{
	return true;
}

void APlayerControllerBody::ServerSetSomeBool_Implementation(bool bNewSomeBool)
{
	// This function is only called on the server (where Role == ROLE_Authority), called over the network by clients.
	// We need to call SetSomeBool() to actually change the value of the bool now!
	// Inside that function, Role == ROLE_Authority, so it won't try to call ServerSetSomeBool() again.
	SetSomeBool(bNewSomeBool);
}