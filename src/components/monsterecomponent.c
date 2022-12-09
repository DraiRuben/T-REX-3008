#include <components/monsterecomponent.h>

#include <components/collectablecomponent.h>
#include <components/inventorycomponent.h>

#include <components/spritecomponent.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	H3Handle player;
	H3Handle scene;
	H3Handle camera;

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
		H3Handle energy = H3_Object_Create2(props->scene, "first energy", NULL, 15);
		H3_Object_AddComponent(energy, SPRITECOMPONENT_CREATE("assets/UI_Energy.png", 0x22));
		H3_Object_SetTranslation(energy, 100, 100);

		//consumes object and remove to the inventory
		InventoryComponent_SetObjSlot2Ex(props->player, NULL);
		H3_Object_Destroy(object, false);
	}
}

void* MonstereComponent_CreateProperties(H3Handle playerRef, H3Handle cameraRef, H3Handle sceneRef)
{
	MonstereComponent_Properties* properties = malloc(sizeof(MonstereComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->player	= playerRef;
	properties->camera	= cameraRef;
	properties->scene	= sceneRef;


	return properties;
}