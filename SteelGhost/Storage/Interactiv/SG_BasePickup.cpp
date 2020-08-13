// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_BasePickup.h"
#include "Net/UnrealNetwork.h"
#include "Volums/BaseSpawnVolume.h"
//#include "Components/StaticMeshComponent.h"

/*
bool ASG_BasePickup::CanUseItem_Implementation()
{
	return true;
}

void ASG_BasePickup::SelectItem_Implementation()
{
	InterractMesh->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
}
*/

// Sets default values
ASG_BasePickup::ASG_BasePickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		// Create the static mesh component
	BoxPickUp =		CreateDefaultSubobject<UBoxComponent>(TEXT("BoxPickUp"));
	RootComponent = BoxPickUp;

	PickupMesh =	CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->AttachTo(RootComponent);

	InterractMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InterractMesh"));
	InterractMesh->AttachTo(RootComponent);
	//InterractMesh->SetVisibility(true);
	//RootComponent = InterractMesh;
	//InterractMesh->SetupAttachment(RootComponent);

	HelperMesh =	CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelpMesh"));
	HelperMesh->SetVisibility(false, false);
	HelperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HelperMesh->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	HelperMesh->AttachTo(RootComponent);

	//InterractMesh->AttachTo(HelperMesh);
	//RootComponent = InterractMesh;

	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = true;

	//if (TSubclassOfTestPTR != nullptr)
	//	if (TestPTR7 == nullptr)
	//		TestPTR7 = NewObject<USG_BaseItem>(this, TSubclassOfTestPTR, "Test1");
}

void ASG_BasePickup::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASG_BasePickup, Item);
}

// Called when the game starts or when spawned
void ASG_BasePickup::BeginPlay()
{
	Super::BeginPlay();

	if (TSubclassOfTestPTR != nullptr)
		if (!TestPTR7->IsValidLowLevel())
			TestPTR7 = NewObject<USG_BaseItem>(this, TSubclassOfTestPTR, "Test1");

	if (Role == ROLE_Authority)
	{
		if (ToSpawnItem)
		{
			if (Ptr_Item != nullptr)
			{
				//Item = NewObject<USG_BaseItem>(Ptr_ItemClass);
				//ItemClass->GetClass()
				//if (Ptr_Item->IsValidLowLevelFast())
				{
					if (!Item->IsValidLowLevel())
					{
						Item = NewObject<USG_BaseItem>(this, Ptr_Item);
					}
				//Item = NewObject<USG_BaseItem>(Ptr_Item->GetClass());
				//Item = NewObject<USG_BaseItem>(this, ClassToCreate->GetName(), RF_NoFlags, ClassToCreate->GetDefaultObject());
				}
			}
		}
	}
}

// Called every frame
void ASG_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role < ROLE_Authority)
	{
		if (IsSelected)
		{
			CurentTimeSelected += DeltaTime;
			if (CurentTimeSelected >= MaxTimeSelected)
			{
				IsSelected = false;
				if (InterractMesh)
				{
					InterractMesh->SetCustomDepthStencilValue(0);
					InterractMesh->SetRenderCustomDepth(false);
				}
				if (HelperMesh)
				{
					HelperMesh->SetVisibility(false);
				}
			}
		}
	}
}

void ASG_BasePickup::Destroyed()
{
	if (this->GetOwner())
	{

	}
}

void ASG_BasePickup::SelectPickup()
{	
	if (Role < ROLE_Authority)
	{
		IsSelected = true;
		CurentTimeSelected = 0.0f;
		if (InterractMesh)
		{
			InterractMesh->SetRenderCustomDepth(true);
			InterractMesh->SetCustomDepthStencilValue(250);
		}
		if (HelperMesh)
		{
			HelperMesh->SetWorldRotation(FRotator(0, 0, 0));
			HelperMesh->SetWorldLocation(this->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));
			HelperMesh->SetVisibility(true, false);
		}
	}
}



bool ASG_BasePickup::PickItem()
{	
	return false;
}

void ASG_BasePickup::OnDestroy()
{
	if (Role == ROLE_Authority)
	{
		AActor* ActorOwner = this->GetOwner();
		if (ActorOwner != nullptr)
		{
			ABaseSpawnVolume* SpawnVolune = Cast<ABaseSpawnVolume>(ActorOwner);
			if (SpawnVolune != nullptr)
			{
				SpawnVolune->DicrimentCountItem();
			}

		}
		//вызвать на сервере событие уничтожение обьекта
	}
	this->Destroy();
}


void ASG_BasePickup::OnDestroyNetMulticast_Implementation()
{
	this->Destroy();
}
/*
void ASG_BasePickup::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		// Turn off the light  
		//PointLight->SetVisibility(false);
	}
}*/