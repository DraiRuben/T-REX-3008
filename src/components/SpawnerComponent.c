#include "components/SpawnerComponent.h"

#include "components/spritecomponent.h"
#include "components/EnemyComponent.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <h3.h>

typedef struct
{
	int tempEnemy_index;
	bool* GlobalAggro;
	bool* IsWave;
	bool* IsNewWave;
	float x, y;
	//raycast
	int raycast_index;
	//enemies
	char enemies[256];
	int enemy_index;
	int timer;
	int amount;
	//ref to player, scene and enrgyBar
	H3Handle* player;
	H3Handle* GameScene;
	H3Handle energyBar;

} SpawnerComponent_Properties;


void SpawnerComponent_Terminate(void* properties)
{
	free(properties);
}
//player pos in map
float px, py;
//distance between player and enemy for vector normalization
float distance;
int i = 0;
int u = 0;

void SpawnerComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	SpawnerComponent_Properties* props = (SpawnerComponent_Properties*)properties;
	//for normal spawn
	H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &px, &py);
	while (i < props->amount)
	{
		H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &props->x, &props->y);
		distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));
		while (distance < 500.0f) {
			distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));
			props->y = rand() % 2150 + 300;
			//if in reserve
			if (props->y < 1100) {
				props->x = rand() % 1400 + 100;
			}
			//if in shop
			else {
				props->x = rand() % 1700 + 100;
			}
			//just in case
			distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));

		}
		snprintf(props->enemies, 256, "enemy_%d", props->enemy_index++);
		H3Handle enemy = H3_Object_Create2(*props->GameScene, props->enemies, NULL, 3);
		H3_Object_EnablePhysics(enemy, H3_BOX_COLLIDER(CDT_Dynamic, 20, 30, 0x22, false));
		H3_Object_AddComponent(enemy, SPRITECOMPONENT_CREATE("assets/Sprites/p.png", 0x22));
		H3_Object_AddComponent(enemy, ENEMYCOMPONENT_CREATE(props->player, &props->raycast_index, props->GameScene, props->energyBar, props->IsNewWave, props->GlobalAggro, false));
		H3_Object_SetTranslation(enemy, props->x, props->y);

		i += 1;
		//ui je sais c'est pas bien de faire dormir le programme
		Sleep(1);
	}		
	//for wave every hour
	if (*props->IsNewWave) {
		props->amount += 2;
		*props->GlobalAggro = true;
		for (u = 0; u < 5; u++) {
			H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &props->x, &props->y);
			distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));
			while (distance > 800.0f || distance<200.0f ) {
				distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));
				props->y = rand() % 2150 + 300;
				//if in reserve
				if (props->y < 1100) {
					props->x = rand() % 1400 + 100;
				}
				//if in shop
				else {
					props->x = rand() % 1700 + 100;
				}
				//just in case
				distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));

			}
			snprintf(props->enemies, 256, "enemy_%d", props->enemy_index++);
			H3Handle enemy = H3_Object_Create2(*props->GameScene, props->enemies, NULL, 3);
			H3_Object_EnablePhysics(enemy, H3_BOX_COLLIDER(CDT_Dynamic, 20, 30, 0x22, false));
			H3_Object_AddComponent(enemy, SPRITECOMPONENT_CREATE("assets/Sprites/p.png", 0x22));
			H3_Object_AddComponent(enemy, ENEMYCOMPONENT_CREATE(props->player, &props->raycast_index, props->GameScene, props->energyBar, props->IsNewWave, props->GlobalAggro,true));
			H3_Object_SetTranslation(enemy, props->x, props->y);
			Sleep(1);
		}
		*props->IsNewWave = false;
	}
	
}


void* SpawnerComponent_CreateProperties(H3Handle* player, H3Handle* GameScene, H3Handle energyBarRef,bool* IsNewWave,bool* IsWave,bool* GlobalAggro)
{
	SpawnerComponent_Properties* properties = malloc(sizeof(SpawnerComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->GlobalAggro = GlobalAggro;
	properties->IsWave = IsWave;
	properties->IsNewWave = IsNewWave;
	properties->energyBar = energyBarRef;
	properties->GameScene = GameScene;
	properties->player = player;
	properties->enemy_index = 0;
	properties->timer = 0;
	properties->amount = 3;
	properties->x = 1750;
	properties->y = 2100;
	properties->raycast_index = 0;
	return properties;
}

void SpawnerCollision(H3Handle object, SH3Collision obj_id) {
	SH3Component* component = H3_Object_GetComponent(object, SPAWNERCOMPONENT_TYPEID);
	SpawnerComponent_Properties* props = (SpawnerComponent_Properties*)(component->properties);

}

