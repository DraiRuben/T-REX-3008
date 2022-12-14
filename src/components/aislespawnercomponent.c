//todo: donner un nom plus représentatif


#include "components/AisleSpawnerComponent.h"
#include "components/spritecomponent.h"
#include "components/collectablecomponent.h"
#include "components/pentagramcomponent.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
	int aisleID;
	char schoolAisle[100];
	char uselessAisle[100];
	char monsterAisle[100];
	char bakeryAisle[100];
	char butcherAisle[100];
	char fishAisle[100];
	char Door[100];
	bool IsInitialized;
	bool hasCrowBarSpawned;
	char AisleTempName[256];
	int AisleTempIndex;
	//ref to scene and player
	H3Handle* Player;
	H3Handle* GameScene;

} AisleSpawnerComponent_Properties;


void AisleSpawnerComponent_Terminate(void* properties)
{
	free(properties);
}

H3Handle Pentagram1;
H3Handle Pentagram2;
H3Handle Pentagram3;
H3Handle Pentagram4;
int limit = 100;
bool IsMonsterInitialized = false;
float px, py;

void AisleSpawnerComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	AisleSpawnerComponent_Properties* props = (AisleSpawnerComponent_Properties*)properties;
	H3_Transform_GetPosition(H3_Object_GetTransform(*props->Player), &px, &py);
	if (!props->IsInitialized) {
		//big random Aisles
		for (int i = 0; i < 5; i++) {
			for (int u = 0; u < 3;u++) {
				props->AisleTempIndex += 1;
				snprintf(props->AisleTempName, 256, "Aisle_%d", props->AisleTempIndex);
				H3Handle Aisle = H3_Object_Create2(*props->GameScene, props->AisleTempName, NULL, 5);
				props->aisleID = rand() % limit;
				if (!(i > 3 && u > 1 && !IsMonsterInitialized)) {
					if (props->aisleID <= 100 && props->aisleID>90) {
						limit -= 10;
						H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->monsterAisle, 0x11));
						H3_Object_AddComponent(Aisle, COLLECTABLECOMPONENT_CREATE(1,33,NULL));
						H3_Object_EnablePhysics(Aisle, H3_BOX_COLLIDER(CDT_Dynamic, 264, 82, 0x11, true));
						IsMonsterInitialized = true;
					}
					else if (props->aisleID <= 90 && props->aisleID>75) {
						H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->schoolAisle, 0x11));
						H3_Object_AddComponent(Aisle, COLLECTABLECOMPONENT_CREATE(3, 15,NULL));
						H3_Object_EnablePhysics(Aisle, H3_BOX_COLLIDER(CDT_Dynamic, 264, 82, 0x11, true));
					}
					else if (props->aisleID <= 75) {
						H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->uselessAisle, 0x11));
					}
				}
				else {
					limit -= 10;
					H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->monsterAisle, 0x11));
					H3_Object_AddComponent(Aisle, COLLECTABLECOMPONENT_CREATE(1,33,NULL));
					H3_Object_EnablePhysics(Aisle, H3_BOX_COLLIDER(CDT_Dynamic, 264, 82, 0x11, true));
					IsMonsterInitialized = true;
				}

				if (i < 4) {
					H3_Object_SetTranslation(Aisle, 598 + 288 * u, 1286 + 192 * i);
				}
				else {
					H3_Object_SetTranslation(Aisle, 598 + 288 * u, 2150);

				}
			}
		}
		//baguettes
		for (int i = 0; i < 3; i++) {
			props->AisleTempIndex += 1;
			snprintf(props->AisleTempName, 256, "Aisle_%d", props->AisleTempIndex);
			H3Handle Aisle = H3_Object_Create2(*props->GameScene, props->AisleTempName, NULL, 5);
			H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->bakeryAisle, 0x11));
			H3_Object_AddComponent(Aisle, COLLECTABLECOMPONENT_CREATE(5, 8,NULL));
			H3_Object_EnablePhysics(Aisle, H3_BOX_COLLIDER(CDT_Dynamic, 144, 80, 0x11, true));
			H3_Object_SetTranslation(Aisle, 824 + i * 192, 2256);
		}
		//meat
		for (int i = 0; i < 3; i++) {
			for (int u = 0; u < 2; u++) {
				props->AisleTempIndex += 1;
				snprintf(props->AisleTempName, 256, "Aisle_%d", props->AisleTempIndex);
				H3Handle Aisle = H3_Object_Create2(*props->GameScene, props->AisleTempName, NULL, 5);
				H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->butcherAisle, 0x11));
				H3_Object_AddComponent(Aisle, COLLECTABLECOMPONENT_CREATE(7, 4,NULL));
				H3_Object_EnablePhysics(Aisle, H3_BOX_COLLIDER(CDT_Dynamic, 96, 48, 0x11, true));
				H3_Object_SetTranslation(Aisle, 80 + u * 160, 1616+ i*96);
			}
		}
		//fish
		for (int i = 0; i < 2; i++) {
			for (int u = 0; u < 3; u++) {
				props->AisleTempIndex += 1;
				snprintf(props->AisleTempName, 256, "Aisle_%d", props->AisleTempIndex);
				H3Handle Aisle = H3_Object_Create2(*props->GameScene, props->AisleTempName, NULL, 5);
				H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->fishAisle, 0x11));
				H3_Object_AddComponent(Aisle, COLLECTABLECOMPONENT_CREATE(10, 2,NULL));
				H3_Object_EnablePhysics(Aisle, H3_BOX_COLLIDER(CDT_Dynamic, 96, 48, 0x11, true));
				H3_Object_SetTranslation(Aisle, 48 + u * 128, 2256 + i * 116);
			}
		}
		//reserve door
		H3Handle Door = H3_Object_Create2(*props->GameScene, "Door", NULL, 5);
		H3Handle DoorColl = H3_Object_Create2(*props->GameScene, "DoorColl", NULL, 5);
		H3_Object_AddComponent(Door, SPRITECOMPONENT_CREATE(props->Door, 0x11));
		H3_Object_AddComponent(Door, COLLECTABLECOMPONENT_CREATE(12, 2, DoorColl));
		H3_Object_EnablePhysics(Door, H3_BOX_COLLIDER(CDT_Static, 110, 100, 0x11, true));
		H3_Object_EnablePhysics(DoorColl, H3_BOX_COLLIDER(CDT_Static, 90, 58, 0x11, false));
		H3_Object_SetTranslation(DoorColl, 58, 1120);
		H3_Object_SetTranslation(Door, 58, 1120);

		//exit door
		H3Handle ExitDoorTrig = H3_Object_Create2(*props->GameScene, "ExitDoorTrigger", NULL, 5);
		H3_Object_AddComponent(ExitDoorTrig, COLLECTABLECOMPONENT_CREATE(13, 1, NULL));
		H3_Object_EnablePhysics(ExitDoorTrig, H3_BOX_COLLIDER(CDT_Static, 80, 80, 0x22, true));
		H3_Object_SetTranslation(ExitDoorTrig, 1168, 64);
		
		//pentagrams
		Pentagram1 = H3_Object_Create2(*props->GameScene, "pentagram1", NULL, 2);
		H3_Object_AddComponent(Pentagram1, SPRITECOMPONENT_CREATE("assets/Sprites/pentagram1.png", 0x11));
		H3_Object_AddComponent(Pentagram1, PENTAGRAMCOMPONENT_CREATE("assets/Sprites/pentagram1A.png"));
		H3_Object_EnablePhysics(Pentagram1, H3_BOX_COLLIDER(CDT_Static, 64, 72, 0x11, true));
		H3_Object_SetTranslation(Pentagram1, 800, 408);
		Pentagram2 = H3_Object_Create2(*props->GameScene, "pentagram2", NULL, 2);
		H3_Object_AddComponent(Pentagram2, SPRITECOMPONENT_CREATE("assets/Sprites/pentagram2.png", 0x11));
		H3_Object_AddComponent(Pentagram2, PENTAGRAMCOMPONENT_CREATE("assets/Sprites/pentagram2A.png"));
		H3_Object_EnablePhysics(Pentagram2, H3_BOX_COLLIDER(CDT_Static, 62, 66, 0x11, true));
		H3_Object_SetTranslation(Pentagram2, 800, 530);
		Pentagram3 = H3_Object_Create2(*props->GameScene, "pentagram3", NULL, 2);
		H3_Object_AddComponent(Pentagram3, SPRITECOMPONENT_CREATE("assets/Sprites/pentagram3.png", 0x11));
		H3_Object_AddComponent(Pentagram3, PENTAGRAMCOMPONENT_CREATE("assets/Sprites/pentagram3A.png"));
		H3_Object_EnablePhysics(Pentagram3, H3_BOX_COLLIDER(CDT_Static, 46, 48, 0x11, true));
		H3_Object_SetTranslation(Pentagram3, 810, 650);
		Pentagram4 = H3_Object_Create2(*props->GameScene, "pentagram4", NULL, 2);
		H3_Object_AddComponent(Pentagram4, SPRITECOMPONENT_CREATE("assets/Sprites/pentagram4.png", 0x11));
		H3_Object_AddComponent(Pentagram4, PENTAGRAMCOMPONENT_CREATE("assets/Sprites/pentagram4A.png"));
		H3_Object_EnablePhysics(Pentagram4, H3_BOX_COLLIDER(CDT_Static, 66, 62, 0x11, true));
		H3_Object_SetTranslation(Pentagram4, 816, 722);
		props->IsInitialized = true;
	}
	//pied de biche, autant le faire à un endroit où on a une référence aux 4 pentagrammes
	if (PentagramComponent_GetIsActivatedEx(Pentagram1)
		&& PentagramComponent_GetIsActivatedEx(Pentagram2)
		&& PentagramComponent_GetIsActivatedEx(Pentagram3)
		&& PentagramComponent_GetIsActivatedEx(Pentagram4)
		&& !props->hasCrowBarSpawned) {
		H3Handle crowbar = H3_Object_Create2(*props->GameScene, "crowbar", NULL, 3);
		H3_Object_AddComponent(crowbar, SPRITECOMPONENT_CREATE("assets/Objects/PiedDeBiche.png", 0x22));
		H3_Object_AddComponent(crowbar, COLLECTABLECOMPONENT_CREATE(14, 1, NULL));
		H3_Object_EnablePhysics(crowbar, H3_BOX_COLLIDER(CDT_Dynamic, 16, 16, 0x22, true));
		H3_Object_SetTranslation(crowbar, px+20, py);
		props->hasCrowBarSpawned = true;
	}
}


void* AisleSpawnerComponent_CreateProperties(H3Handle* GameScene,H3Handle* Player)
{
	AisleSpawnerComponent_Properties* properties = malloc(sizeof(AisleSpawnerComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	AisleSpawnerComponent_Properties* props = (AisleSpawnerComponent_Properties*)properties;
	properties->AisleTempIndex = 0;
	properties->Player = Player;
	properties->GameScene = GameScene;
	properties->IsInitialized = false;
	properties->hasCrowBarSpawned = false;
	//why did I do that ? I don't even know myself
	sprintf_s(props->uselessAisle, 100, "assets/map/rayonpif.png");
	sprintf_s(props->schoolAisle, 100, "assets/map/rayonscolaire.png");
	sprintf_s(props->monsterAisle, 100, "assets/map/rayonmonster.png");
	sprintf_s(props->bakeryAisle, 100, "assets/map/rayonboulangerie.png");
	sprintf_s(props->butcherAisle, 100, "assets/map/rayonboucherie.png");
	sprintf_s(props->fishAisle, 100, "assets/map/rayonpoisson.png");
	sprintf_s(props->Door, 100, "assets/map/DoorClose.png");
	return properties;
}