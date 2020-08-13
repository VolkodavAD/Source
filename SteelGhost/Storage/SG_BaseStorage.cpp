// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_BaseStorage.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USG_BaseStorage::USG_BaseStorage()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TestRep = 1;
	SizeX = 4;
	SizeY = 4;
	ItemArray.SetNumZeroed(SizeX*SizeY);
	// ...

}

void USG_BaseStorage::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USG_BaseStorage, TestRep);
	DOREPLIFETIME(USG_BaseStorage, ItemArray);
	//DOREPLIFETIME(AReplicatedActor, bFlag);
	//DOREPLIFETIME(AReplicatedActor, IntegerArray);
}

void USG_BaseStorage::InitStorage(uint8 NewSizeX, uint8 NewSizeY)
{
	SizeX = NewSizeX;
	SizeY = NewSizeY;
	ItemArray.SetNumZeroed(SizeX*SizeY);
}

// Called when the game starts
void USG_BaseStorage::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USG_BaseStorage::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool USG_BaseStorage::CheckCellFull(int32 CheckedCell, int32 ItemSizeX, int32 ItemSizeY)
{
	int32 ix = CheckedCell % SizeX;
	int32 iy = CheckedCell / SizeX;
	UE_LOG(LogTemp, Log, TEXT("check cell -  X- %d  Y- %d"), ix, iy);
	// результат проверки на зан€тые €чейки
	bool Local_FreeArea = true;
	for (uint8 Local_iy = iy; Local_iy < (iy + ItemSizeY); ++Local_iy)
	{
		for (uint8 Local_ix = ix; Local_ix < (ix + ItemSizeX); ++Local_ix)
		{
			int32 LocalActivIndex = Local_ix + (Local_iy * SizeX);

			UE_LOG(LogTemp, Log, TEXT("cell %d is %s"), LocalActivIndex, (ItemArray[LocalActivIndex].IsFull ? TEXT("True") : TEXT("False")));

			if (ItemArray[Local_iy*SizeX + Local_ix].IsFull == true)
			{
				Local_FreeArea = false;
				break;
			}
		}
	}
	return Local_FreeArea;
}

int32 USG_BaseStorage::GetFreeSlot(int32 ItemSizeX, int32 ItemSizeY)
{
	// первичный цикл проверки €чеек
	for (uint8 iy = 0; iy < (SizeY - ItemSizeY + 1); ++iy)
	{
		for (uint8 ix = 0; ix < (SizeX - ItemSizeX + 1); ++ix)
		{
			if (CheckCellFull(iy * SizeX + ix, ItemSizeX, ItemSizeY))
			{
				return (int32)(iy* SizeX + ix);
			}
		}
	}
	return -1;
}

bool USG_BaseStorage::AddItemInSlot(USG_BaseItem* AddedItem)
{
	if (AddedItem != nullptr)
	{
		int32 IndexFreeSlot = GetFreeSlot(AddedItem->ItemInfo.SizeX, AddedItem->ItemInfo.SizeY);
		int32 ItemSizeX = AddedItem->ItemInfo.SizeX;
		int32 ItemSizeY = AddedItem->ItemInfo.SizeY;

		if (IndexFreeSlot >= 0)
		{
			for (int32 iy = 0; iy < ItemSizeY; iy++)
			{
				for (int32 ix = 0; ix < ItemSizeX; ix++)
				{
					int32 LocalActivIndex = IndexFreeSlot + ix + (iy * SizeX);

					FStorageSlot ActivSlot;

					ActivSlot.IndexInStorage = ItemArray[LocalActivIndex].IndexInStorage;
					ActivSlot.SlotType = ItemArray[LocalActivIndex].SlotType;

					ActivSlot.IsFull = true;
					ActivSlot.Item = AddedItem;
					//UE_LOG(LogTemp, Log, TEXT("item %d set %s"), LocalActivIndex, AddedItem->GetName());

					ActivSlot.IndexLocalItem_X = ix;
					ActivSlot.IndexLocalItem_Y = iy;

					ItemArray[LocalActivIndex] = ActivSlot;

					UE_LOG(LogTemp, Log, TEXT("item %d set %s"), LocalActivIndex, (ItemArray[LocalActivIndex].IsFull ? TEXT("True") : TEXT("False")));
					//ItemArray[LocalActivIndex].IsFull = true;

					//[IndexFreeSlot + ix + (iy * SizeX)].IsFull = true;
				}
			}
			//ASG_BaseItem NewItem;
			//NewItem.ItemId = 111;
			//NewItem.ItemType = EItemType::Item_BodyPart;
			//[IndexFreeSlot].IsFull = true;
			//ItemArray[IndexFreeSlot].Item = AddedItem;
		}
		else
		{
			return false;
		}
		return true;
	}
	else return false;
}

bool USG_BaseStorage::DeleteItemFromSlot(USG_BaseItem* AddedItem)
{
	return false;
}
