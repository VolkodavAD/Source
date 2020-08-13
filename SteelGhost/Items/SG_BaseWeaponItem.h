// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/SG_BaseItem.h"
#include "SG_BaseWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class STEELGHOST_API USG_BaseWeaponItem : public USG_BaseItem
{
	GENERATED_BODY()

public:
	USG_BaseWeaponItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSG_WeaponInfo WiaponItemInfo;
};