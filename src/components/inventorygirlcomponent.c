#include <components/inventorygirlcomponent.h>

#include <components/spritecomponent.h>
#include <components/collectablecomponent.h>
#include <components/monsterecomponent.h>
#include <components/mancomponent.h>
#include <components/womancomponent.h>
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
	H3Handle ObjSlot2;	
	H3Handle ObjSlot3;	//hand
	H3Handle OpenDoor;
	H3Handle HitSFX;
	uint32_t w, h;
	H3Handle DoorOpenSFX;
	H3Handle* EnergyBar;
	H3Handle* GameScene;
	bool IsMan;
} InventoryGirlComponent_Properties;

void InventoryGirlComponent_Terminate(void* properties)
{
	free(properties);
}

char girlObject[256];
int girlObject_index = 0;

void InventoryGirlComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	InventoryGirlComponent_Properties* props = (InventoryGirlComponent_Properties*)properties;

	//get player pos
	H3_Transform_GetPosition(H3_Object_GetTransform(object), &props->playerX, &props->playerY);

	//drop an object
	if (H3_Input_IsKeyPressed(K_Space) && props->ObjSlot3 != NULL)
	{
		CollectableComponent_SetisInHandEx(props->ObjSlot3, false);

		float angle = rand() % 361;
		H3_Object_SetTranslation(props->ObjSlot3, props->playerX, props->playerY);
		H3_Object_Rotate(props->ObjSlot3, angle);
		H3_Object_SetRenderOrder(props->ObjSlot3, 2);
		props->ObjSlot3 = NULL;
	}

	//change the object in hand with an object in inventory
	if (H3_Input_IsMouseBtnPressed(MB_Right))
	{
		H3Handle temporarySlot3 = props->ObjSlot3;

		props->ObjSlot3 = props->ObjSlot2;
		props->ObjSlot2 = props->ObjSlot1;
		props->ObjSlot1 = props->ObjSlot0;
		props->ObjSlot0 = temporarySlot3;
	}

	//add an object in the hand
	if (H3_Input_IsMouseBtnPressed(MB_Left) &&
		props->triggerObj != NULL &&
		props->ObjSlot3 == NULL
		&& CollectableComponent_GettypeEx(props->triggerObj) != 12
		&& CollectableComponent_GettypeEx(props->triggerObj) != 13)
	{
		//monster Aisle
		if (CollectableComponent_GettypeEx(props->triggerObj) == 1) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(girlObject, 256, "object_%d", girlObject_index++);
			H3Handle Monster = H3_Object_Create2(*props->GameScene, girlObject, NULL, 12);
			H3_Object_AddComponent(Monster, SPRITECOMPONENT_CREATE("assets/Objects/monstère.png", 0x22));
			H3_Object_AddComponent(Monster, COLLECTABLECOMPONENT_CREATE(2, 1, NULL));
			H3_Object_AddComponent(Monster, MONSTERECOMPONENT_CREATE(object, *props->EnergyBar, props->IsMan));
			H3_Object_EnablePhysics(Monster, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot3 = Monster;
		}
		//book Aisle
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 3) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(girlObject, 256, "object_%d", girlObject_index++);
			H3Handle Book = H3_Object_Create2(*props->GameScene, girlObject, NULL, 12);
			H3_Object_AddComponent(Book, SPRITECOMPONENT_CREATE("assets/Objects/book.png", 0x22));
			H3_Object_AddComponent(Book, COLLECTABLECOMPONENT_CREATE(4, 1, NULL));
			H3_Object_AddComponent(Book, PROJECTILECOMPONENT_CREATE(object, &props->HitSFX, props->IsMan));
			H3_Object_EnablePhysics(Book, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot3 = Book;
		}
		//bakery Aisle
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 5) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(girlObject, 256, "object_%d", girlObject_index++);
			H3Handle Baguette = H3_Object_Create2(*props->GameScene, girlObject, NULL, 12);
			H3_Object_AddComponent(Baguette, SPRITECOMPONENT_CREATE("assets/Objects/baguette.png", 0x22));
			H3_Object_AddComponent(Baguette, COLLECTABLECOMPONENT_CREATE(6, 1, NULL));
			H3_Object_AddComponent(Baguette, PROJECTILECOMPONENT_CREATE(object, &props->HitSFX, props->IsMan));
			H3_Object_EnablePhysics(Baguette, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot3 = Baguette;
		}

		//butcher Aisle
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 7) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(girlObject, 256, "object_%d", girlObject_index++);
			H3Handle Meat = H3_Object_Create2(*props->GameScene, girlObject, NULL, 12);
			H3_Object_AddComponent(Meat, SPRITECOMPONENT_CREATE("assets/Objects/meat.png", 0x22));
			H3_Object_AddComponent(Meat, COLLECTABLECOMPONENT_CREATE(8, 1, NULL));
			H3_Object_AddComponent(Meat, PROJECTILECOMPONENT_CREATE(object, &props->HitSFX, props->IsMan));
			H3_Object_EnablePhysics(Meat, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot3 = Meat;
		}
		//fish Aisle
		else if (CollectableComponent_GettypeEx(props->triggerObj) == 10) {
			CollectableComponent_SetdurabilityEx(props->triggerObj, CollectableComponent_GetdurabilityEx(props->triggerObj) - 1);
			snprintf(girlObject, 256, "object_%d", girlObject_index++);
			H3Handle Fish = H3_Object_Create2(*props->GameScene, girlObject, NULL, 12);
			H3_Object_AddComponent(Fish, SPRITECOMPONENT_CREATE("assets/Objects/fish.png", 0x22));
			H3_Object_AddComponent(Fish, COLLECTABLECOMPONENT_CREATE(11, 1, NULL));
			H3_Object_AddComponent(Fish, PROJECTILECOMPONENT_CREATE(object, &props->HitSFX, props->IsMan));
			H3_Object_EnablePhysics(Fish, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			props->ObjSlot3 = Fish;
		}
		//object on the ground
		else if (( CollectableComponent_GettypeEx(props->triggerObj) == 11
				|| CollectableComponent_GettypeEx(props->triggerObj) == 8
				|| CollectableComponent_GettypeEx(props->triggerObj) == 6
				|| CollectableComponent_GettypeEx(props->triggerObj) == 4)
				&& !ProjectileComponent_GetIsLaunchedEx(props->triggerObj)) {
				props->ObjSlot3 = props->triggerObj;

				props->nbTrigger--;
				if (props->nbTrigger <= 0) {
					props->nbTrigger = 0;
					props->triggerObj = NULL;
				}
		}
		else if (  CollectableComponent_GettypeEx(props->triggerObj) == 2
				|| CollectableComponent_GettypeEx(props->triggerObj) == 9
				|| CollectableComponent_GettypeEx(props->triggerObj) == 14) {
				props->ObjSlot3 = props->triggerObj;

				props->nbTrigger--;
				if (props->nbTrigger <= 0) {
					props->nbTrigger = 0;
					props->triggerObj = NULL;
				}
				if (CollectableComponent_GettypeEx(props->ObjSlot3) == 2)
					MonstereComponent_SetisReadyToUseEx(props->ObjSlot3, false);
		}
		CollectableComponent_SetisInHandEx(props->ObjSlot3, true);
	}

	//open the reserve door
	if (H3_Input_IsMouseBtnPressed(MB_Left)
		&& props->triggerObj != NULL
		&& props->ObjSlot3 != NULL
		&& CollectableComponent_GettypeEx(props->ObjSlot3) == 9
		&& CollectableComponent_GettypeEx(props->triggerObj) == 12)
	{
		H3_Sound_Play(props->DoorOpenSFX, 0.3, false);
		SpriteComponent_SetTextureEx(props->triggerObj, props->OpenDoor);
		H3_Object_Destroy(CollectableComponent_GetDoorCollEx(props->triggerObj), false);
		CollectableComponent_SetdurabilityEx(props->ObjSlot3, 0);
		props->ObjSlot3 = NULL;
	}

	//open the exit door
	if (H3_Input_IsMouseBtnPressed(MB_Left)
		&& props->triggerObj != NULL
		&& props->ObjSlot3 != NULL
		&& CollectableComponent_GettypeEx(props->ObjSlot3) == 14
		&& CollectableComponent_GettypeEx(props->triggerObj) == 13)
	{
		WomanComponent_SetplayerWinEx(object, true);
		CollectableComponent_SetdurabilityEx(props->ObjSlot3, 0);
		props->ObjSlot3 = NULL;
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
		CollectableComponent_SetisInHandEx(props->ObjSlot2, false);
		H3_Object_SetTranslation(props->ObjSlot2, (props->playerX + 150), (props->playerY - 115));
	}
	if (props->ObjSlot3 != NULL) {
		CollectableComponent_SetisInHandEx(props->ObjSlot3, true);
		H3_Object_SetTranslation(props->ObjSlot3, (props->playerX + 115), (props->playerY - 115));
		H3_Object_SetRenderOrder(props->ObjSlot3, 12);
	}
}


void InventoryGirlComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties) {
	InventoryGirlComponent_Properties* props = (InventoryGirlComponent_Properties*)properties;
	/*if (props->ObjSlot2 != NULL) {
		H3_Texture_Draw(h3, props->playerX+5, props->playerY-10, SpriteComponent_GetTextureEx(props->ObjSlot2), 0x22);
	}*/
}

//register trigger object and trigger count
void InventoryGirlComponent_OnTriggerEnter(H3Handle object, SH3Collision collision)
{
	SH3Component* component = H3_Object_GetComponent(object, INVENTORYGIRLCOMPONENT_TYPEID);
	InventoryGirlComponent_Properties* props = (InventoryGirlComponent_Properties*)component->properties;

	if (collision.other != NULL)
	{
		if (H3_Object_HasComponent(collision.other, COLLECTABLECOMPONENT_TYPEID))
		{
			//projectile 
			if (   CollectableComponent_GettypeEx(collision.other) == 4
				|| CollectableComponent_GettypeEx(collision.other) == 6
				|| CollectableComponent_GettypeEx(collision.other) == 8
				|| CollectableComponent_GettypeEx(collision.other) == 11)
			{
				//not launched
				if (!ProjectileComponent_GetIsLaunchedEx(collision.other)) {
					props->triggerObj = collision.other;
					props->nbTrigger++;
				}
			}
			//others
			else {
				props->triggerObj = collision.other;
				props->nbTrigger++;
			}
		}
	}
}

void InventoryGirlComponent_OnTriggerLeave(H3Handle object, H3Handle other)
{
	SH3Component* component = H3_Object_GetComponent(object, INVENTORYGIRLCOMPONENT_TYPEID);
	InventoryGirlComponent_Properties* props = (InventoryGirlComponent_Properties*)component->properties;

	if (other != NULL)
	{
		if (H3_Object_HasComponent(other, COLLECTABLECOMPONENT_TYPEID))
		{
			//projectile 
			if (CollectableComponent_GettypeEx(other) == 4
				|| CollectableComponent_GettypeEx(other) == 6
				|| CollectableComponent_GettypeEx(other) == 8
				|| CollectableComponent_GettypeEx(other) == 11)
			{
				//not launched
				if (!ProjectileComponent_GetIsLaunchedEx(other)) {
					props->nbTrigger--;
				}
			}
			//others
			else {
				props->nbTrigger--;
			}
		}
	}

	//reset trigger object
	if (props->nbTrigger <= 0) {
		props->nbTrigger = 0;
		props->triggerObj = NULL;
	}
}

void* InventoryGirlComponent_CreateProperties(H3Handle* GameScene, H3Handle* EnergyBar, bool IsMan)
{
	InventoryGirlComponent_Properties* properties = malloc(sizeof(InventoryGirlComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	
	properties->IsMan = IsMan;

	properties->nbTrigger = 0;
	properties->triggerObj = NULL;
	properties->ObjSlot0 = NULL;
	properties->ObjSlot1 = NULL;
	properties->ObjSlot2 = NULL;
	properties->ObjSlot3 = NULL;
	properties->EnergyBar = EnergyBar;
	properties->GameScene = GameScene;
	properties->DoorOpenSFX = H3_Sound_Load("assets/SFX/DoorOpenSFX.wav");
	properties->HitSFX = H3_Sound_Load("assets/SFX/ProjectileHitSFX.wav");
	properties->OpenDoor = H3_Texture_Load("assets/map/DoorOpen.png", &properties->w, &properties->h);
	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(InventoryGirlComponent, INVENTORYGIRLCOMPONENT_TYPEID, H3Handle, ObjSlot3); //access to the object in hand