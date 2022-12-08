#include "components/EnemyComponent.h"
#include "components/spritecomponent.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
	bool IsAggro;
	float x, y;
	float timer;
	float timer2;
	float timer3;
	int* raycast_index;
	int direction;
	char raycasts[256];
	H3Handle raycasting;
	H3Handle* player;
	H3Handle* GameScene;
} EnemyComponent_Properties;


void EnemyComponent_Terminate(void* properties)
{
	free(properties);
}
float px, py;
void EnemyComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	EnemyComponent_Properties* props = (EnemyComponent_Properties*)properties;
	H3_Transform_GetPosition(transform, &props->x, &props->y);
	H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &px, &py);
	//raycast create
	if (props->raycasting == NULL) {
		*props->raycast_index += 1;
		snprintf(props->raycasts, 256, "raycast_%d", *props->raycast_index);
		props->raycasting = H3_Object_Create(*props->GameScene, props->raycasts, NULL);
		//H3_Object_AddComponent(props->raycasting, RAYCASTCOMPONENT_CREATE());
		H3_Object_EnablePhysics(props->raycasting, H3_BOX_COLLIDER(CDT_Dynamic, 150, 50, 0x22, true));
	}

	//idle
	if (props->IsAggro == false) {
		//timers
		props->timer3 += H3_GetDeltaTime();
		props->timer += H3_GetDeltaTime();
		if (props->timer3 > 7) {
			props->direction = ((rand() % 4) + 1);
			props->timer3 = 0;
		}
		//move periodically
		if (props->timer > 2) {
			if (props->direction == 1) {
				H3_Object_SetVelocity(object, -50, 0);
			}
			else if (props->direction == 2) {
				H3_Object_SetVelocity(object, 50, 0);
			}
			else if (props->direction == 3) {
				H3_Object_SetVelocity(object, 0, -50);
			}
			else if (props->direction == 4) {
				H3_Object_SetVelocity(object, 0, 50);
			}
			props->timer2 += H3_GetDeltaTime();
			if (props->timer2 > 3) {
				H3_Object_SetVelocity(object, 0, 0);
				props->timer2 = 0;
				props->timer = 0;
			}
		}
		//raycast check obstacle at certain disance between player and bot
		
	}

	//aggro'd
	/*if (props->IsAggro == true) {
		H3_Object_SetVelocity(object, (px - props->x) / sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y)) * 120,
			(py - props->y) / sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y)) * 120);
	}*/

}


void* EnemyComponent_CreateProperties(H3Handle* GameScene, int* raycast_index, H3Handle* player)
{
	EnemyComponent_Properties* properties = malloc(sizeof(EnemyComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->timer3 = 8;
	properties->GameScene = GameScene;
	properties->raycast_index = raycast_index;
	properties->timer = 0;
	properties->timer2 = 0;
	properties->IsAggro = false;
	properties->raycasting = NULL;
	properties->player = player;
	return properties;
}

void EnemyCollision(H3Handle object, SH3Collision obj_id) {
	SH3Component* component = H3_Object_GetComponent(object, ENEMYCOMPONENT_TYPEID);
	EnemyComponent_Properties* props = (EnemyComponent_Properties*)(component->properties);

}

