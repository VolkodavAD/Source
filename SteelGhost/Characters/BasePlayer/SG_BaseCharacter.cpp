// Fill out your copyright notice in the Description page of Project Settings.

#include "SG_BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h" 	
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "GameFramework/Actor.h"
#include "Characters/BasePlayer/SG_BasePlayerState.h"
#include "SteelGhostGameMode.h"
#include "PlayerControllerBody.h"
#include "Kismet/GameplayStatics.h"
#include "Storage/Interactiv/SG_BasePickup.h"

// Sets default values
ASG_BaseCharacter::ASG_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm Component"); 
	SpringArmComponent->bUsePawnControlRotation = true;   
	//SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	
	bReplicates = true;
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true; 
	FirstPersonCameraComponent->SetupAttachment(SpringArmComponent);

	CharacterStorage = CreateDefaultSubobject<USG_BaseStorage>(TEXT("CharacterStorage"));
	//CharacterStorage->SetupAttachment(GetCapsuleComponent());
	//CharacterStorage->SetIsReplicated(true);

	// Сфера для проверки пересечения и анализа вокруг игрока
	CharacterSphereConponent = CreateDefaultSubobject<USphereComponent>(TEXT("CharacterSphere"));
	CharacterSphereConponent->AttachTo(RootComponent);
	CharacterSphereConponent->SetSphereRadius(10.0f);

	//TestStorage->InitStorage(4, 4);


	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	MeshBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	MeshBody->SetupAttachment(GetMesh());
	//Mesh1P->SetOnlyOwnerSee(true);
	//MeshBody->SetupAttachment(GetCapsuleComponent());
	MeshBody->bCastDynamicShadow = false;
	MeshBody->CastShadow = false;
	MeshBody->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	MeshBody->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	MeshFP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	//MeshFP_Gun->SetupAttachment(MeshBody, TEXT("GripPoint"));
	//MeshFP_Gun->SetupAttachment(GetMesh(), TEXT("GripPoint"));
	MeshFP_Gun->SetupAttachment(GetMesh());
	MeshFP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	MeshFP_Gun->bCastDynamicShadow = false;
	MeshFP_Gun->CastShadow = false;

	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Hands"));
	Hands->SetupAttachment(GetMesh());

	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Legs"));
	Legs->SetupAttachment(GetMesh());
}

void ASG_BaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASG_BaseCharacter, ChatTestRep);
	DOREPLIFETIME(ASG_BaseCharacter, FirstPersonCameraComponent);
	DOREPLIFETIME_CONDITION(ASG_BaseCharacter, CharacterStorage, COND_SkipOwner);
}

// Called when the game starts or when spawned
void ASG_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();	
	GetPlayerState();
	//GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASG_BaseCharacter::TraseForward, 0.0333f, true, 1.0f);
	UE_LOG(LogTemp, Log, TEXT("BeginPlay character UniqueID %d"), this->GetUniqueID());
}

// Called when the game starts or when spawned
void ASG_BaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

// Called every frame
void ASG_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraseForward();

	if (ToUpdateStorageWitget)
	{
		CurrentTimeToUpdateStorageWitget += DeltaTime;
		if (CurrentTimeToUpdateStorageWitget > MaxTimeToUpdateStorageWitget)
		{
			ToUpdateStorageWitget = false;
			UpdateStorageWitget();
			CurrentTimeToUpdateStorageWitget = 0.0f;
		}
	}
}

// Called to bind functionality to input
void ASG_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASG_BaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASG_BaseCharacter::MoveRight);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASG_BaseCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ASG_BaseCharacter::LookUpAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// Bind use event
	PlayerInputComponent->BindAction("StrafeLeft", IE_Pressed, this, &ASG_BaseCharacter::StrafeLeft);
	PlayerInputComponent->BindAction("StrafeRight", IE_Pressed, this, &ASG_BaseCharacter::StrafeRight);
	PlayerInputComponent->BindAction("StrafeLeft", IE_Released, this, &ASG_BaseCharacter::StrafeLeft);
	PlayerInputComponent->BindAction("StrafeRight", IE_Released, this, &ASG_BaseCharacter::StrafeRight);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASG_BaseCharacter::ToScan);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ASG_BaseCharacter::OnUse);
	PlayerInputComponent->BindAction("SwitchMode", IE_Released, this, &ASG_BaseCharacter::SwitchMode);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASG_BaseCharacter::OnFire);
	//StrafeLeft
	//StrafeRight

	//PlayerInputComponent->BindAxis("LookUp", this, &ASG_BaseCharacter::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("Turn", this, &ASG_BaseCharacter::AddControllerYawInput);
}

/** нанесение урона игроку */
float ASG_BaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (Health <= 0.f)
		{
			Health = 0.0f;
			//SetLifeSpan(0.001f);
			Death();
		}
	}

	return ActualDamage;
}

bool ASG_BaseCharacter::Death()
{
	if (HasAuthority())
	{
		IsLive = false;

		UWorld* world = GetWorld();
		if (world)
		{
			if (world->GetAuthGameMode() != nullptr)
			{
				ASteelGhostGameMode* GM = Cast<ASteelGhostGameMode>(world->GetAuthGameMode());
				ASG_BasePlayerController* CTR = Cast<ASG_BasePlayerController>(this->GetController());
				
				GM->DeadPlayerCharacter(this);
				//GetController()->UnPossess();
			}
		}
	}
	return true;
}

void ASG_BaseCharacter::UpdateStorageWitget_Implementation(){}
//функция трейса перед игроком для выделения обьектов и т.д.
void ASG_BaseCharacter::TraseForward()
{
	if (Role < ROLE_Authority)
	{
		//  linetrase к обьекту
		FHitResult OutHit;
		//FP_Gun->GetComponentLocation();

			// alternatively you can get the camera location
			// FVector Start = FirstPersonCameraComponent->GetComponentLocation();

		FVector Start = FirstPersonCameraComponent->GetComponentLocation();
		FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
		ForwardVector = this->GetControlRotation().Vector();
		FVector End = ((ForwardVector * 1000.f) + Start);
		FCollisionQueryParams CollisionParams;

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Camera, CollisionParams))
		{
			if (OutHit.bBlockingHit)
			{
				AActor *HitActor = OutHit.GetActor();

				ASG_BasePickup* HitPikcupActor = Cast<ASG_BasePickup>(HitActor);
				if (HitPikcupActor)
				{
					HitPikcupActor->SelectPickup();
				}

				/*
				USG_ItemsInterface* HitInterface = Cast<USG_ItemsInterface>(HitActor);
				if (HitInterface)
				{
					//Don't call your functions directly, use the 'Execute_' prefix
					//the Execute_ReactToHighNoon and Execute_ReactToMidnight are generated on compile
					//you may need to compile before these functions will appear
				}
				*/

				ACharacter* Hit_BaseCharacter = Cast<ACharacter>(HitActor);
				if (Hit_BaseCharacter)
				{
					if ((OutHit.BoneName == "head")||(OutHit.BoneName == "neck_01"))
					{
						UE_LOG(LogTemp, Log, TEXT("head"));
					}
					if (OutHit.BoneName == "hand_l"||"lowerarm_l"||"upperarm_l")
					{
						UE_LOG(LogTemp, Log, TEXT("hand_l"));
					}
					if (OutHit.BoneName == "hand_l"|| "lowerarm_r"|| "upperarm_r")
					{
						UE_LOG(LogTemp, Log, TEXT("hand_r"));
					}
					Hit_BaseCharacter->GetMesh();					
				}
			}
			// draw line to Hit rnd point
			//MulticastDrawDebugLine(OutHit);
			//DrawDebugLine(GetWorld(), Start, OutHit.TraceEnd, FColor::Green, false, 0.5f, 0, 0.1f);
			//DrawDebugBox(GetWorld(), OutHit.ImpactPoint, OutHit.Normal, FColor::Green, false, 0.5f, 0, 0.5f);
		}
	}
}

void ASG_BaseCharacter::OnUse()
{
	// тестовая добавление предметов
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	USG_BaseItem* AddedItem = NewObject<USG_BaseItem>(USG_BaseItem::StaticClass());
	/*
	AddedItem->ItemInfo.BaseItemInfo.ItemId = 1;
	AddedItem->ItemInfo.BaseItemInfo.ItemType = ESG_ItemType::Item_BodyPart;
	AddedItem->ItemInfo.SizeX = 2;
	AddedItem->ItemInfo.SizeY = 2;
	AddedItem->ItemInfo.ItemName = "TestItem";
	*/
	//CharacterStorage->AddItem(AddedItem);
	
	/*
	// вывод в лог предметов в инвентаре
	for (size_t i = 0; i < CharacterStorage->ItemArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("item %d is %s"), i, (CharacterStorage->ItemArray[i].IsFull ? TEXT("True") : TEXT("False")));
	}

	if (Role == ROLE_Authority)
	{
		CharacterStorage->TestRep = 2;
	}
	*/

	SetSomeBool(true);
	
	//  linetrase к обьекту
	FHitResult OutHit;
	//FP_Gun->GetComponentLocation();

		// alternatively you can get the camera location
		// FVector Start = FirstPersonCameraComponent->GetComponentLocation();

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	ForwardVector = this->GetControlRotation().Vector();
	FVector End = ((ForwardVector * 1000.f) + Start);
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			/*
			AActor *HitActor = OutHit.GetActor();

			ASG_BasePickup* HitPikcupActor = Cast<ASG_BasePickup>(HitActor);			
			if (HitPikcupActor)
			{
				HitPikcupActor->SelectPickup();
			}

			USG_ItemsInterface* TheInterface = Cast<USG_ItemsInterface>(HitActor);
			if (TheInterface)
			{
				//Don't call your functions directly, use the 'Execute_' prefix
				//the Execute_ReactToHighNoon and Execute_ReactToMidnight are generated on compile
				//you may need to compile before these functions will appear
			}
			*/
			OnUseServer();
			MaxTimeToUpdateStorageWitget = 0.333f;
			ToUpdateStorageWitget = true;
			UpdateStorageWitget();
		}
		// draw line to Hit rnd point
		DrawDebugLine(GetWorld(), Start, OutHit.TraceEnd, FColor::Green, false, 1, 0, 1);
	}
}
void ASG_BaseCharacter::OnUseServer_Implementation()
{
	//  linetrase к обьекту
	FHitResult OutHit;
	//FP_Gun->GetComponentLocation();

		// alternatively you can get the camera location
		// FVector Start = FirstPersonCameraComponent->GetComponentLocation();

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	ASG_BaseCharacter* Character = Cast<ASG_BaseCharacter>(this->GetInstigator());
	ForwardVector = this->GetControlRotation().Vector();
	if (this->GetOwner() != nullptr)
	{
		ForwardVector = this->GetOwner()->GetActorForwardVector();
	}
	//ForwardVector = Character->FirstPersonCameraComponent->GetForwardVector();
	FVector End = ((ForwardVector * 1000.f) + Start);
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{

			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString())); 

			UE_LOG(LogTemp, Warning, TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString());
			//....

			AActor *HitActor = OutHit.GetActor();

			ASG_BasePickup* HitPikcupActor = Cast<ASG_BasePickup>(HitActor);
			if (HitPikcupActor)
			{
				//HitPikcupActor->Destroy();
				//HitPikcupActor->PickItem();
				/*
				if (CharacterStorage->AddItem(HitPikcupActor->Item))
				{
					CharacterStorage->TestRep = 3;
					HitPikcupActor->OnDestroy();
				}
				*/
				//try put in our storage
				//CharacterStorage->AddItem(HitPikcupActor.P)
			}

			/*
			USG_ItemsInterface* TheInterface = Cast<USG_ItemsInterface>(HitActor);
			if (TheInterface)
			{
				//TheInterface.
				//Don't call your functions directly, use the 'Execute_' prefix
				//the Execute_ReactToHighNoon and Execute_ReactToMidnight are generated on compile
				//you may need to compile before these functions will appear
				//TheInterface->CanUse
			}
			*/

			/*
			if (OutHit.GetActor()->GetClass()->ImplementsInterface(USG_ItemsInterface::StaticClass()))
			{
				//*OutHit.GetActor()->Execute_ReactToHighNoon(pointerToAnyUObject);
				//USG_ItemsInterface::Execute_GetPickUpItem();
			}
			*/

		}
		// draw line to Hit rnd point
		MulticastDrawDebugLine(OutHit);
		//DrawDebugLine(GetWorld(), Start, OutHit.TraceEnd, FColor::Green, false, 1, 0, 1);
	}
}
bool ASG_BaseCharacter::OnUseServer_Validate()
{
	return true;
}

void ASG_BaseCharacter::SwitchMode()
{
	APlayerControllerBody* PC = Cast<APlayerControllerBody>(this->Controller);
	if (PC != nullptr)
	{
		ASG_BasePlayerState* PS = Cast<ASG_BasePlayerState>(PC->PlayerState);
		SwitchModeServer();
		UE_LOG(LogTemp, Log, TEXT("PlayerBodyState %d"), PS->PlayerBodyState);
	}

	//ASG_BasePlayerState* PS = Cast<ASG_BasePlayerState>(UGameplayStatics::GetGameState(GetWorld()));
}
void ASG_BaseCharacter::SwitchModeServer_Implementation()
{
	// получаем контроллер игрока
	APlayerControllerBody* PC = Cast<APlayerControllerBody>(this->Controller);
	if (PC)
	{
		ASG_BasePlayerState* PS = Cast<ASG_BasePlayerState>(PC->PlayerState);
		if (PS != nullptr)
		{
			if (PS->PlayerBodyState < 2)
			{
				PS->PlayerBodyState += 1;
			}
			else
			{
				PS->PlayerBodyState = 0;
			}
		}
	}
}
bool ASG_BaseCharacter::SwitchModeServer_Validate()
{
	return true;
}

void ASG_BaseCharacter::MulticastDrawDebugLine_Implementation(FHitResult OutHit)
{
	DrawDebugLine(GetWorld(), OutHit.TraceStart, OutHit.TraceEnd, FColor::Green, false, 0.5f, 0, 0.5f);
	DrawDebugBox(GetWorld(), OutHit.ImpactPoint, FVector(20.0f), OutHit.Normal.ToOrientationQuat(), FColor::Yellow, false, 0.5f, 0, 0.5f);
}

void ASG_BaseCharacter::SetSomeBool(bool bNewSomeBool)
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

bool ASG_BaseCharacter::ServerSetSomeBool_Validate(bool bNewSomeBool)
{
	return true;
}

void ASG_BaseCharacter::ServerSetSomeBool_Implementation(bool bNewSomeBool)
{
	// This function is only called on the server (where Role == ROLE_Authority), called over the network by clients.
	// We need to call SetSomeBool() to actually change the value of the bool now!
	// Inside that function, Role == ROLE_Authority, so it won't try to call ServerSetSomeBool() again.
	SetSomeBool(bNewSomeBool);
	ChatTestRep = ChatTestRep + 1;
}

void ASG_BaseCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		//AddControllerYawInput(0.1f);
		//AddControllerPitchInput(0.1f);
	}
}
void ASG_BaseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}


float LenghtStrafe = 1000.0f;
void ASG_BaseCharacter::StrafeRight()
{
	StrafeServer(1.0f);
	//FHitResult* StrafeHit;
	//this->SetActorLocation(this->GetActorLocation() + (this->GetActorRightVector*LenghtStrafe*Value), false, StrafeHit, ETeleportType::None);
}
void ASG_BaseCharacter::StrafeLeft()
{
	StrafeServer(-1.0f);
	//FHitResult* StrafeHit;
	//this->SetActorLocation(this->GetActorLocation() + (this->GetActorRightVector*LenghtStrafe*Value), false, StrafeHit, ETeleportType::None);
}

void ASG_BaseCharacter::StrafeServer_Implementation(float Rate)
{
	FHitResult StrafeHit;
	//this->SetActorLocation(this->GetActorLocation() + (this->GetActorRightVector()*LenghtStrafe*Rate), false, &StrafeHit, ETeleportType::None);
	this->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}
bool ASG_BaseCharacter::StrafeServer_Validate(float Rate)
{
	return true;
}

void ASG_BaseCharacter::TurnAtRate(float Rate)
{
	if (Rate > 0.2f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("TurnAtRate: %f"), Rate);
	}
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(Rate);
	//TurnAtRateServer(Rate);
}
void ASG_BaseCharacter::TurnAtRateServer_Implementation(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
bool ASG_BaseCharacter::TurnAtRateServer_Validate(float Rate)
{
	return true;
}

void ASG_BaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	//LookUpAtRateServer(Rate);
}
void ASG_BaseCharacter::LookUpAtRateServer_Implementation(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	AController* Contr = GetController();
	//this.
}
bool ASG_BaseCharacter::LookUpAtRateServer_Validate(float Rate)
{
	return true;
}

// сканирование области вокруг
void ASG_BaseCharacter::ToScan()
{
	//получить все предметы внутри области
	TArray<AActor*> CollectedActors;
	CharacterSphereConponent->GetOverlappingActors(CollectedActors);
	//для каждого предмета включить выделение
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		ASG_BasePickup* const TestPickup = Cast<ASG_BasePickup>(CollectedActors[iCollected]);
		if (TestPickup && !TestPickup->IsPendingKill())
		{
			TestPickup->SelectPickup();
		}
	}
}


void ASG_BaseCharacter::OnFire()
{
	//  linetrase к обьекту
	FHitResult OutHit;
	//FP_Gun->GetComponentLocation();

	// alternatively you can get the camera location
	// FVector Start = FirstPersonCameraComponent->GetComponentLocation();

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	ForwardVector = this->GetControlRotation().Vector();
	FVector End = ((ForwardVector * 2000.f) + Start);
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			OnFireServer();
		}
		// draw line to Hit rnd point
		//DrawDebugLine(GetWorld(), Start, OutHit.TraceEnd, FColor::Green, false, 1, 0, 1);
	}
}

void ASG_BaseCharacter::OnFireServer_Implementation()
{
	//  linetrase к обьекту
	FHitResult OutHitCamera;
	//FP_Gun->GetComponentLocation();

	// alternatively you can get the camera location
	// FVector Start = FirstPersonCameraComponent->GetComponentLocation();

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	ASG_BaseCharacter* Character = Cast<ASG_BaseCharacter>(this->GetInstigator());
	ForwardVector = this->GetControlRotation().Vector();
	if (this->GetOwner() != nullptr)
	{
		ForwardVector = this->GetOwner()->GetActorForwardVector();
	}
	//ForwardVector = Character->FirstPersonCameraComponent->GetForwardVector();
	FVector End = ((ForwardVector * 2000.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	float MyDamage = 100;
	if (GetWorld()->LineTraceSingleByChannel(OutHitCamera, Start, End, ECC_Camera, CollisionParams))
	{
		//если трейс от камеры блокирован
		if (OutHitCamera.bBlockingHit)
		{
			//GetSocketLoatoin

			const float DamageAmount = 60.0f;
			bool CheckHeadSocket = (GetMesh()->DoesSocketExist("HeadSocket"));
			Start = GetMesh()->GetSocketLocation("HeadSocket");

			//if (GetMesh()->DoesSocketExist("HeadSocket"))
			//	Start = GetMesh()->GetSocketLocation("HeadSocket");
			//else
			
			//Start = GetMesh()->GetComponentToWorld().GetLocation();
			End = Start + ((OutHitCamera.TraceEnd - Start).GetSafeNormal() * 2000.0f);
			//End = Start + (ForwardVector * 20000.0f);

			FHitResult OutHit;
			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Camera, CollisionParams))
			{

				AActor *HitActor = OutHit.GetActor();

				ACharacter* HitPikcupActor = Cast<ACharacter>(HitActor);
				if (HitPikcupActor)
				{
					// Create a damage event  
					//TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
					//FDamageEvent DamageEvent(ValidDamageTypeClass);
					UGameplayStatics::ApplyPointDamage(HitPikcupActor, DamageAmount, OutHit.TraceStart, OutHit, GetController(), this, nullptr);
					//tPikcupActor->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
					//HitPikcupActor->InternalTakePointDamage(DamageAmount, DamageEvent, GetController(), this);
				}

				/*
				USG_ItemsInterface* TheInterface = Cast<USG_ItemsInterface>(HitActor);
				if (TheInterface)
				{
					//TheInterface.
					//Don't call your functions directly, use the 'Execute_' prefix
					//the Execute_ReactToHighNoon and Execute_ReactToMidnight are generated on compile
					//you may need to compile before these functions will appear
					//TheInterface->CanUse
				}
				*/

				/*
				if (OutHit.GetActor()->GetClass()->ImplementsInterface(USG_ItemsInterface::StaticClass()))
				{
					//*OutHit.GetActor()->Execute_ReactToHighNoon(pointerToAnyUObject);
					//USG_ItemsInterface::Execute_GetPickUpItem();
				}
				*/
			}
			MulticastDrawDebugLine(OutHit);
		}
		// draw line to Hit rnd point
		//DrawDebugLine(GetWorld(), Start, OutHit.TraceEnd, FColor::Green, false, 1, 0, 1);
	}
}
bool ASG_BaseCharacter::OnFireServer_Validate()
{
	return true;
}