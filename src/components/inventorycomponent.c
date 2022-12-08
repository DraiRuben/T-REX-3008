#include <components/inventorycomponent.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	float playerX;
	float playerY;
	int nbTrigger;
	H3Handle triggerObj;
	H3Handle ObjSlot0;
	H3Handle ObjSlot1;
	H3Handle ObjSlot2;
} InventoryComponent_Properties;

void InventoryComponent_Terminate(void* properties)
{
	free(properties);
}

void InventoryComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	InventoryComponent_Properties* props = (InventoryComponent_Properties*)properties;
	
	//get player pos
	H3_Transform_GetPosition(H3_Object_GetTransform(object), &props->playerX, &props->playerY);

	//add an object in inventory
	if (props->triggerObj != NULL && H3_Input_IsMouseBtnPressed(MB_Left))
	{
		if (props->ObjSlot0 == NULL) {
			props->ObjSlot0 = props->triggerObj;
			props->triggerObj = NULL;
		}
		else if (props->ObjSlot1 == NULL) {
			props->ObjSlot1 = props->triggerObj;
			props->triggerObj = NULL;
		}
		else if (props->ObjSlot2 == NULL) {
			props->ObjSlot2 = props->triggerObj;
			props->triggerObj = NULL;
		}
	}

	//let object in the right slot
	if (props->ObjSlot0 != NULL) {
		H3_Object_SetTranslation(props->ObjSlot0, (props->playerX + 220), (props->playerY - 115));
		H3_Object_SetRenderOrder(props->ObjSlot0, 12);
	}
	if (props->ObjSlot1 != NULL) {
		H3_Object_SetTranslation(props->ObjSlot1, (props->playerX + 185), (props->playerY - 115));
		H3_Object_SetRenderOrder(props->ObjSlot1, 12);
	}
	if (props->ObjSlot2 != NULL) {
		H3_Object_SetTranslation(props->ObjSlot2, (props->playerX + 150), (props->playerY - 115));
		H3_Object_SetRenderOrder(props->ObjSlot2, 12);
	}
}

void InventoryComponent_OnTriggerEnter(H3Handle object, SH3Collision collision)
{
	SH3Component* component = H3_Object_GetComponent(object, INVENTORYCOMPONENT_TYPEID);
	InventoryComponent_Properties* props = (InventoryComponent_Properties*) component->properties;

	props->triggerObj = collision.other;
	props->nbTrigger++;
}

void InventoryComponent_OnTriggerLeave(H3Handle object, H3Handle other)
{
	SH3Component* component = H3_Object_GetComponent(object, INVENTORYCOMPONENT_TYPEID);
	InventoryComponent_Properties* props = (InventoryComponent_Properties*)component->properties;
	
	props->nbTrigger--;
	if (props->nbTrigger == 0)
		props->triggerObj = NULL;
}

void* InventoryComponent_CreateProperties()
{
	InventoryComponent_Properties* properties = malloc(sizeof(InventoryComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->nbTrigger = 0;
	properties->triggerObj = NULL;
	properties->ObjSlot0 = NULL;
	properties->ObjSlot1 = NULL;
	properties->ObjSlot2 = NULL;


	return properties;
}