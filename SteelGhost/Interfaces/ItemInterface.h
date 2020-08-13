// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/BasePlayer/SG_BaseCharacter.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IItemInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Получение признака того, что объект может быть использован */
	virtual bool CanBeUsed(ASG_BaseCharacter* Character) = 0;

	/** Использовать объект */
	virtual void Use(ASG_BaseCharacter* Character) = 0;

	/** Получение признака того, что объект может быть Астивно */
	virtual bool CanBeActiv(ASG_BaseCharacter* Character) = 0;

	/** Активация объекта */
	virtual void Activ(ASG_BaseCharacter* Character) = 0;
};
