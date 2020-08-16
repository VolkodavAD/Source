// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Character.h"
#include "Storage/SG_Storage.h"
#include "Storage/Storage/BaseStorage/TestActorComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "SG_BaseCharacter.generated.h"


class UInputComponent;

UCLASS(config = Game)
class STEELGHOST_API ASG_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* MeshBody;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* MeshFP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Hands;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Legs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USpringArmComponent * SpringArmComponent;

	/** First person camera */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CharacterSphereConponent;

	/** Movement component used for movement logic in various movement modes (walking, falling, etc), containing relevant settings and functions to control movement. */
	//UPROPERTY(Replicated, Category = Storage, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//	UCharacterMovementComponent* CharacterMovement;


	UPROPERTY(Replicated, Category = Storage, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USG_Storage* Storage;

	UPROPERTY(Replicated, Category = Storage, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTestActorComponent* TestComponent;

	FTimerHandle SpawnTimer;

public:
	// Sets default values for this character's properties
	ASG_BaseCharacter();

	/**тестовая переменная для проверки репликации */
	UPROPERTY(Replicated, EditAnywhere)
		uint8 ChatTestRep;
	
	/**тестовая переменная для проверки репликации */
	UPROPERTY(Replicated, EditAnywhere)
		uint8 SSS[5] {0, 5, 7, 9, 4};


	/** задержки для обновление инвенторя. надо ПЕРЕДЕЛАТЬ но onrep */
	/** Через сколько обновить инвентарь */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Storage)
		float MaxTimeToUpdateStorageWitget;
	/** текуцее время задержки */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Storage)
		float CurrentTimeToUpdateStorageWitget;
	/** следует обновить инвентарь*/
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Storage)
		bool ToUpdateStorageWitget;	

	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic)
		void UpdateStorageWitget();

	
	/** признак облушения персонажа */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Bogy")
		bool HaveStan = false;
	/** общее время оглушения */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Bogy")
		float MaxTimeStan = 0.0f;
	/** прошедшее время оглушения */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Bogy")
		float CurrentTimeStan = 0.0f;

	/** контроллер упрвляет персонажем */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Bogy")
		bool PlayerHaveControl = true;

	/** контроллер упрвляет персонажем */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Bogy")
		bool IsLive = true;

	/** здоровье */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Bogy")
		float Health = 100.0f;

	/** части тела персонажа */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bogy")
	//	TArray< USG_CharacterBodyParts* > MyBodyParts;

protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	/** функция получения урона */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);
	
	/** функция смерти персонажа */
	virtual bool Death();

	/** функция сканирования: подсветки pickup обьектов ит т.д.*/
	UFUNCTION(BlueprintCallable, Category = "PickUp")
	void ToScan();
	/** linetrase to forward */
	virtual void SwitchMode();
	UFUNCTION(Server, Reliable, WithValidation)
	void SwitchModeServer();

	/** linetrase to forward */
	virtual void OnUse();
	UFUNCTION(Server, Reliable, WithValidation)
	void OnUseServer();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastDrawDebugLine(FHitResult OutHit);
	
	/** Fires a projectile. */
	void OnFire();
	UFUNCTION(Server, Reliable, WithValidation)
	void OnFireServer();

	// ---------------------------------------------------------------- Управленте персонажем ---------------------------------------------------------------
	/** Функции управления*/
	/** Handles moving forward/backward */
	void MoveForward(float Val);
	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void StrafeLeft();
	void StrafeRight();
	UFUNCTION(Server, Reliable, WithValidation)
	void StrafeServer(float Rate);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	virtual void TurnAtRate(float Rate);
	UFUNCTION(Server, Reliable, WithValidation)
	void TurnAtRateServer(float Rate);
	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	virtual void LookUpAtRate(float Rate);
	UFUNCTION(Server, Reliable, WithValidation)
	void LookUpAtRateServer(float Rate);

	//функция трейса перед игроком для выделения обьектов и т.д.
	void TraseForward();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate = 100.0f;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate = 100.0f;

	bool bSomeBool = false;
	void SetSomeBool(bool bNewSomeBool);

protected:
	UFUNCTION(reliable, server, WithValidation)
	void ServerSetSomeBool(bool bNewSomeBool);

public:

	/** Returns Mesh1P subobject **/
	//FORCEINLINE class USkeletalMeshComponent* Get() const { return MeshBody; }

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USG_Storage* GetStorage() const { return Storage; }

	FORCEINLINE class UTestActorComponent* GetTestComponent() const { return TestComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FORCEINLINE class USphereComponent* GetCharacterSphereConponent() const { return CharacterSphereConponent; }
};
