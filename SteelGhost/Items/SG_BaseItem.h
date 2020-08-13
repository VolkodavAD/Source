// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SG_BaseItem.generated.h"
/**
 * 
 */

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESG_ItemType : uint8
{
	Item_Universal 	UMETA(DisplayName = "Universal"),
	Item_Resourse	UMETA(DisplayName = "Resource"),
	Item_Core		UMETA(DisplayName = "Core"),
	Item_BodyPart 	UMETA(DisplayName = "BodyPart"),
	Item_Device		UMETA(DisplayName = "Device"),
	Item_Container	UMETA(DisplayName = "Container")
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESG_WeapomType : uint8
{
	Weapon_Melee 	UMETA(DisplayName = "Melee"),
	Weapon_Pistol	UMETA(DisplayName = "Pistol"),
	Weapon_Assault	UMETA(DisplayName = "Assault")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESG_BodyPartType : uint8
{
	BodyPart_Head 	UMETA(DisplayName = "Head"),
	BodyPart_Tors	UMETA(DisplayName = "Tors"),
	BodyPart_Legs	UMETA(DisplayName = "Legs"),
	BodyPart_ArmL	UMETA(DisplayName = "ArmL"),
	BodyPart_ArmR	UMETA(DisplayName = "ArmR")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESG_AmmoType : uint8
{
	AmmoType_PhysicKinematic 	UMETA(DisplayName = "Ammo_PhysicKinematic"),
	AmmoType_Electric			UMETA(DisplayName = "Ammo_Electric"),
};

// базовые данные о объекте
USTRUCT(BlueprintType)
struct FSG_BaseItemInfo : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemId;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESG_ItemType ItemType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			uint8 SizeX = 1;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			uint8 SizeY = 1;
};

//подробные данные о обьекте
USTRUCT(BlueprintType)
struct FSG_FullItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSG_BaseItemInfo BaseItemInfo;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//UTexture2D* ItemPicture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;

};

// дополнительные данные о ресурсе --------------------------
USTRUCT(BlueprintType)
struct FSG_ResourceInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Balance = 0.0;
};
// объединенные данные о ресурсе
USTRUCT(BlueprintType)
struct FSG_ResourceItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSG_BaseItemInfo FullItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSG_ResourceInfo ResourceInfo;
};

// дополнительные данные о оружие ----------------------------
USTRUCT(BlueprintType)
struct FSG_WeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> AmmorID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCount;
};

// объединенные данные о оружие
USTRUCT(BlueprintType)
struct FSG_WiaponItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSG_BaseItemInfo FullItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSG_WeaponInfo WeaponInfo;
};

// объединенные данные о оружие
USTRUCT(BlueprintType)
struct FSG_AmmoItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSG_BaseItemInfo AmmoDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESG_AmmoType AmmoType;
};

// дополнительные данные о части тела ------------------------
USTRUCT(BlueprintType)
struct FSG_BodyPartInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESG_BodyPartType BodyPartType = ESG_BodyPartType::BodyPart_Tors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConnectionID = 0;
		//EBodyPartType BodyPartType = EBodyPartType::BodyPart_Tors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <ESG_ItemType> FastSlots;
};

// объединенные данные о части тела
USTRUCT(BlueprintType)
struct FSG_BodyPartItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSG_BaseItemInfo FullItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSG_BodyPartInfo BodyPartInfo;
	
};

// =============================================================== Classes =======================================================================
UCLASS(Blueprintable)
class STEELGHOST_API USG_BaseItem : public UObject
{
	GENERATED_BODY()

public:
	USG_BaseItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSG_BaseItemInfo ItemInfo;
};

UCLASS(Blueprintable)
class STEELGHOST_API USG_BaseResourceItem : public USG_BaseItem
{
	GENERATED_BODY()

public:
	USG_BaseResourceItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSG_ResourceInfo ResourceInfo;
};
