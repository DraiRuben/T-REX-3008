#include <components/inventorycomponent.h>
#include <components/spritecomponent.h>
#include <components/collectablecomponent.h>
#include <components/monsterecomponent.h>
#include <components/projectileComponent.h>

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
	H3Handle* EnergyBar;
	H3Handle* GameScene;
} InventoryComponent_Properties;

void InventoryComponent_Terminate(void* properties)
{
	free(properties);
}
char Object[256];
int object_index = 0;
void InventoryComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	InventoryComponent_Properties* props = (InventoryComponent_Properties*)properties;
	
	//get player pos
	H3_Transform_GetPosition(H3_Object_GetTransform(object), &props->playerX, &props->playerY);

	//drop an object
	if (H3_Input_IsKeyPressed(K_Space) && props->ObjSlot2 != NULL)
	{
		CollectableComponent_SetisInHandEx(props->ObjSlot2, false);

		float angle = rand() % 361;
		H3_Object_SetTranslation(props->ObjSlot2, props->playerX, props->playerY);
		H3_Object_Rotate(props->ObjSlot2, angle);
		H3_Object_SetRenderOrder(props->ObjSlot2, 2);
		props->ObjSlot2 = NULL;
	}

	//change the object in hand with an object in inventory
	if (H3_Input_IsMouseBtnPressed(MB_Right))
	{
		H3Handle temporarySlot2 = props->ObjSlot2;

		props->ObjSlot2 = props->ObjSlot1;
		props->ObjSlot1 = props->ObjSlot0;
		props->ObjSlot0 = temporarySlot2;
	}

	//add an object in the hand
	if (H3_Input_IsMouseBtnPressed(MB_Left) && 
		props->triggerObj != NULL && 
		props->ObjSlot2 == NULL)
	{
		//monster Aisle
		if (CollectableComponent_GettypeEx(props->triggerObj) == 1) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(Object, 256, "object_%d", object_index++);
			H3Handle Monster = H3_Object_Create2(*props->GameScene, Object, NULL,12);
			H3_Object_AddComponent(Monster, SPRITECOMPONENT_CREATE("assets/Objects/monstère.png", 0x22));
			H3_Object_AddComponent(Monster, COLLECTABLECOMPONENT_CREATE(2,1));
			H3_Object_AddComponent(Monster, MONSTERECOMPONENT_CREATE(object, *props->EnergyBar));
			H3_Object_EnablePhysics(Monster, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot2 = Monster;
		}
		//monster can
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 2) {
			props->ObjSlot2 = props->triggerObj;
			
		}
		//book Aisle
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 3) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(Object, 256, "object_%d", object_index++);
			H3Handle Book = H3_Object_Create2(*props->GameScene, Object, NULL, 12);
			H3_Object_AddComponent(Book, SPRITECOMPONENT_CREATE("assets/Objects/book.png", 0x22));
			H3_Object_AddComponent(Book, COLLECTABLECOMPONENT_CREATE(4, 1));
			H3_Object_AddComponent(Book, PROJECTILECOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(Book, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot2 = Book;
		}
		//bakery Aisle
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 5) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(Object, 256, "object_%d", object_index++);
			H3Handle Baguette = H3_Object_Create2(*props->GameScene, Object, NULL, 12);
			H3_Object_AddComponent(Baguette, SPRITECOMPONENT_CREATE("assets/Objects/baguette.png", 0x22));
			H3_Object_AddComponent(Baguette, COLLECTABLECOMPONENT_CREATE(6, 1));
			H3_Object_AddComponent(Baguette, PROJECTILECOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(Baguette, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot2 = Baguette;
		}
		
		//butcher Aisle
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 7) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(Object, 256, "object_%d", object_index++);
			H3Handle Meat = H3_Object_Create2(*props->GameScene, Object, NULL, 12);
			H3_Object_AddComponent(Meat, SPRITECOMPONENT_CREATE("assets/Objects/meat.png", 0x22));
			H3_Object_AddComponent(Meat, COLLECTABLECOMPONENT_CREATE(8, 1));
			H3_Object_AddComponent(Meat, PROJECTILECOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(Meat, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot2 = Meat;
		}
		//fish Aisle
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 10) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(Object, 256, "object_%d", object_index++);
			H3Handle Fish = H3_Object_Create2(*props->GameScene, Object, NULL, 12);
			H3_Object_AddComponent(Fish, SPRITECOMPONENT_CREATE("assets/Objects/fish.png", 0x22));
			H3_Object_AddComponent(Fish, COLLECTABLECOMPONENT_CREATE(11, 1));
			H3_Object_AddComponent(Fish, PROJECTILECOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(Fish, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot2 = Fish;
		}
		//object on the ground
		else if ((CollectableComponent_GettypeEx(props->triggerObj) == 11
			||CollectableComponent_GettypeEx(props->triggerObj) == 8 
			|| CollectableComponent_GettypeEx(props->triggerObj) == 6
			|| CollectableComponent_GettypeEx(props->triggerObj) == 4)
			&& !ProjectileComponent_GetIsLaunchedEx(props->triggerObj)) {
			props->ObjSlot2 = props->triggerObj;
		}
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 9) {
			props->ObjSlot2 = props->triggerObj;
		}
		CollectableComponent_SetisInHandEx(props->ObjSlot2, true);
	}

	//let object in the right slot
	if (props->ObjSlot0 != NULL) {
		CollectableComponent_SetisInHandEx(props->ObjSlot0, false);
		H3_Object_SetTranslation(props->ObjSlot0, (props->playerX + 220), (props->playerY - 115));
	}
	if (props->ObjSlot1 != NULL) {
		CollectableComponent_SetisInHandEx(props->ObjSlot1, false);
		H3_Object_SetTranslation(props->ObjSlot1, (props->playerX + 185), (props->playerY - 115));
	}
	if (props->ObjSlot2 != NULL) {
		CollectableComponent_SetisInHandEx(props->ObjSlot2, true);
		H3_Object_SetTranslation(props->ObjSlot2, (props->playerX + 150), (props->playerY - 115));
		if (props->triggerObj!= NULL && CollectableComponent_GettypeEx(props->triggerObj) != 1) {
			H3_Object_SetRenderOrder(props->ObjSlot2, 12);
			props->triggerObj = NULL;
		}
		
	}
	
}


void InventoryComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties) {
	InventoryComponent_Properties* props = (InventoryComponent_Properties*)properties;
	if (props->ObjSlot2 != NULL) {
		H3_Texture_Draw(h3, props->playerX+5, props->playerY-10, SpriteComponent_GetTextureEx(props->ObjSlot2), 0x22);
	}
}


void InventoryComponent_OnTriggerEnter(H3Handle object, SH3Collision collision)
{
	SH3Component* component = H3_Object_GetComponent(object, INVENTORYCOMPONENT_TYPEID);
	InventoryComponent_Properties* props = (InventoryComponent_Properties*) component->properties;

	if (collision.other != NULL)
	{
		if (H3_Object_HasComponent(collision.other, COLLECTABLECOMPONENT_TYPEID))
		{
			if (CollectableComponent_GettypeEx(collision.other) == 4
				|| CollectableComponent_GettypeEx(collision.other) == 6
				|| CollectableComponent_GettypeEx(collision.other) == 8
				|| CollectableComponent_GettypeEx(collision.other) == 11) {
				if (!ProjectileComponent_GetIsLaunchedEx(collision.other)) {
					props->triggerObj = collision.other;
					props->nbTrigger++;
				}
			}
			else if (CollectableComponent_GettypeEx(collision.other) == 9) {
				props->triggerObj = collision.other;
				props->nbTrigger++;
			}
			else {
				props->triggerObj = collision.other;
				props->nbTrigger++;
			}
		}
	}
}

void InventoryComponent_OnTriggerLeave(H3Handle object, H3Handle other)
{
	SH3Component* component = H3_Object_GetComponent(object, INVENTORYCOMPONENT_TYPEID);
	InventoryComponent_Properties* props = (InventoryComponent_Properties*)component->properties;
	
	if (other != NULL)
	{
		if (H3_Object_HasComponent(other, COLLECTABLECOMPONENT_TYPEID))
		{
			if (CollectableComponent_GettypeEx(other) == 4
				|| CollectableComponent_GettypeEx(other) == 6
				|| CollectableComponent_GettypeEx(other) == 8
				|| CollectableComponent_GettypeEx(other) == 11) {
				if (!ProjectileComponent_GetIsLaunchedEx(other)) {
					props->triggerObj = other;
					props->nbTrigger++;
				}
			}
			else if (CollectableComponent_GettypeEx(other) == 9) {
				props->nbTrigger--;
				if (props->nbTrigger == 0)
					props->triggerObj = NULL;
			}
			else {
				props->nbTrigger--;
				if (props->nbTrigger == 0)
					props->triggerObj = NULL;
			}
		}
	}
}

void* InventoryComponent_CreateProperties(H3Handle* GameScene, H3Handle* EnergyBar)
{
	InventoryComponent_Properties* properties = malloc(sizeof(InventoryComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->nbTrigger = 0;
	properties->triggerObj = NULL;
	properties->ObjSlot0 = NULL;
	properties->ObjSlot1 = NULL;
	properties->ObjSlot2 = NULL;
	properties->EnergyBar = EnergyBar;
	properties->GameScene = GameScene;
	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(InventoryComponent, INVENTORYCOMPONENT_TYPEID, H3Handle, ObjSlot2); //access to the object in hand