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
	H3Handle ObjSlot2;	//hand
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

	//drop an object
	if (H3_Input_IsKeyPressed(K_Space) && props->ObjSlot2 != NULL)
	{
		float angle = rand() % 361;
		H3_Object_SetTranslation(props->ObjSlot2, props->playerX, props->playerY);
		H3_Object_Rotate(props->ObjSlot2, angle);
		H3_Object_SetRenderOrder(props->ObjSlot2, 2);
		props->ObjSlot2 = NULL;
	}

	//add an object in the hand
	if (H3_Input_IsMouseBtnPressed(MB_Left) && 
		props->triggerObj != NULL && 
		props->ObjSlot2 == NULL)
	{

		props->ObjSlot2 = props->triggerObj;
		props->triggerObj = NULL;
	}

	//change the object in hand with an object in inventory
	if (H3_Input_IsMouseBtnPressed(MB_Right))
	{
		H3Handle temporarySlot2 = props->ObjSlot2;
		
		props->ObjSlot2 = props->ObjSlot1;
		props->ObjSlot1 = props->ObjSlot0;
		props->ObjSlot0 = temporarySlot2;
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