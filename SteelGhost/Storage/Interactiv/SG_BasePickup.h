// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
//#include "Storage/Storage/SG_BaseStorage.h"
#include "Items/SG_BaseItem.h"
#include "SG_BasePickup.generated.h"


UCLASS(config = Game)
class STEELGHOST_API ASG_BasePickup : public AActor //, public ISG_ItemsInterface
{
	GENERATED_BODY()

protected:
	/** Static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* InterractMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* HelperMesh;

public:	
	// Sets default values for this actor's properties
	ASG_BasePickup();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interfaces)
	bool ReactToPlayerEntered();
	//virtual bool CanUseItem_Implementation() override;

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interfaces)
	void SelectItem();
	//virtual void SelectItem_Implementation() override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//	UStaticlMeshComponent* InterractMesh;

	/** A Replicated Boolean Flag */
	//UPROPERTY(Replicated)
	//	uint32 bFlag : 1;

	/** A Replicated Array Of Integers */
	//UPROPERTY(Replicated)
	//	TArray<uint32> IntegerArray;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* BoxPickUp;

	// класс который надо заспавнить
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ToSpawnItem = true;

	// ссылка на обьект pickup
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class USG_BaseItem* ItemClass;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		USG_BaseItem* Item;

	// м€гкие ссылки
	// обьект класса
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<USG_BaseItem> Ptr_ItemClass;
	// класс который надо заспавнить
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class USG_BaseItem> Ptr_Item;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TSubclassOf<class UTestObject> TSubclassOfTestPTR;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	//	class USG_BaseItem* Class_Item;
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interfaces)
	//	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USG_BaseItem* TestPTR7;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TAssetPtr<USG_BaseItem> TAssetPtr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class USG_BaseItem> TSubclassOfTestPTR;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when the game starts or when spawned
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//virtual bool CanUse() const override;

	UFUNCTION(BlueprintCallable)
	bool PickItem();

	UFUNCTION(BlueprintCallable)
	void SelectPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSelected = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTimeSelected = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurentTimeSelected = 0.0f;

	UFUNCTION(BlueprintCallable)
		void OnDestroy();

	UFUNCTION(NetMulticast, Unreliable)
		void OnDestroyNetMulticast();

	/** Returns Mesh1P subobject **/

	/** Returns Mesh1P subobject **/
	//FORCEINLINE class USG_BaseItem* GetClass_Item() const { return Class_Item; }

	/** Returns Mesh1P subobject **/
	//FORCEINLINE USG_BaseItem* GetPickupItem() const { return Item; }
		/** Return the mesh for the pickup */

	FORCEINLINE class UStaticMeshComponent* GetPickupMesh() const { return PickupMesh; }

	FORCEINLINE class UStaticMeshComponent* GetInterractMesh() const { return InterractMesh; }

	FORCEINLINE class UStaticMeshComponent* GetHelperMesh() const { return HelperMesh; }


};
