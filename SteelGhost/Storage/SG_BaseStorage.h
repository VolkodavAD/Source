// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
//#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/SG_BaseItem.h"
#include "SG_BaseStorage.generated.h"

USTRUCT(BlueprintType)
struct FStorageSlot
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsFull = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESG_ItemType SlotType = ESG_ItemType::Item_Universal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USG_BaseItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 IndexInStorage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 IndexLocalItem_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 IndexLocalItem_Y;
};

//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STEELGHOST_API USG_BaseStorage : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USG_BaseStorage();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		uint8 SizeX = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		uint8 SizeY = 2;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Item")
		TArray<FStorageSlot> ItemArray;


	UPROPERTY(Replicated, EditAnywhere)
		uint8 TestRep;

	// Функции класса
	UFUNCTION(BlueprintCallable, Category = "Item")
		void InitStorage(uint8 NewSizeX, uint8 NewSizeY);

	UFUNCTION(BlueprintCallable, Category = "Item")
		bool CheckCellFull(int32 CheckedCell, int32 ItemSizeX, int32 ItemSizeY);
	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetFreeSlot(int32 ItemSizeX, int32 ItemSizeY);

	UFUNCTION(BlueprintCallable, Category = "Item")
		bool AddItemInSlot(USG_BaseItem* AddedItem);

	UFUNCTION(BlueprintCallable, Category = "Item")
		bool DeleteItemFromSlot(USG_BaseItem* AddedItem);
};
