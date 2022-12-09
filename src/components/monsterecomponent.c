#include <components/monsterecomponent.h>

#include <components/collectablecomponent.h>
#include <components/inventorycomponent.h>
#include <components/tirednesscomponent.h>

#include <components/spritecomponent.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	H3Handle player;
	H3Handle energyBar;


} MonstereComponent_Properties;

void MonstereComponent_Terminate(void* properties)
{
	free(properties);
}

void MonstereComponent_PreUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	MonstereComponent_Properties* props = (MonstereComponent_Properties*)properties;
	
	//use object in hand
	if (H3_Input_IsMouseBtnPressed(MB_Left) && CollectableComponent_GetisInHandEx(object))
	{
		//drink monstere helps to recover energy
		if (TirednessComponent_GetrecovEnergyEx(props->energyBar) == 0)
			TirednessComponent_SetrecovEnergyEx(props->energyBar, 1);
		else if (TirednessComponent_GetrecovEnergyEx(props->energyBar) == 1)
			TirednessComponent_SetrecovEnergyEx(props->energyBar, 2);

		//consumes object and remove to the inventory
		if (TirednessComponent_GetrecovEnergyEx(props->energyBar) < 2)
		{
			InventoryComponent_SetObjSlot2Ex(props->player, NULL);
			H3_Object_Destroy(object, false);
		}
	}
}

void* MonstereComponent_CreateProperties(H3Handle playerRef, H3Handle energyBarRef)
{
	MonstereComponent_Properties* properties = malloc(sizeof(MonstereComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->player		= playerRef;
	properties->energyBar	= energyBarRef;


	return properties;
}