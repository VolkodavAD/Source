// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMetaData.h"
#include "SG_BaseMetaData.generated.h"

/**
 * 
 */
UCLASS()
class STEELGHOST_API USG_BaseMetaData : public UAnimMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		bool bOverrideUseControllerPitch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params, meta = (EditCondition = "bOverrideUseControllerPitch"))
		bool bNewUseControllerPitchValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		bool bOverrideUseControllerYaw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params, meta = (EditCondition = "bOverrideUseControllerYaw"))
		bool bNewUseControllerYawValue = false;

};
