#include "components/EnemyComponent.h"
#include "components/spritecomponent.h"
#include "components/raycastcomponent.h"
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
	float timer4;
	float timer5;
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
float distance;
void EnemyComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties){
EnemyComponent_Properties* props = (EnemyComponent_Properties*)properties;
	H3_Transform_GetPosition(transform, &props->x, &props->y);
	H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &px, &py);
	distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));

	props->timer4 += H3_GetDeltaTime();
	//idle
	if (props->IsAggro == false) {
		//timers
		props->timer += H3_GetDeltaTime();
		props->timer3 += H3_GetDeltaTime();
		
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
		if (distance < 200&&props->timer4>1) {
			*props->raycast_index += 1;
			snprintf(props->raycasts, 256, "ray_%d", *props->raycast_index);
			props->raycasting = H3_Object_Create(*props->GameScene, props->raycasts, NULL);
			H3_Object_AddComponent(props->raycasting, RAYCASTCOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(props->raycasting, H3_BOX_COLLIDER(CDT_Dynamic, 3, 3, 0x22, true));
			
			H3_Object_SetTranslation(props->raycasting, props->x, props->y);
			H3_Object_SetVelocity(props->raycasting, (px - props->x) / distance * 1000, (py - props->y) / distance * 1000);
			props->timer4 = 0;
		}
	}

	//aggro'd
	if (props->IsAggro == true) {
		//follow player
		H3_Object_SetVelocity(object, (px - props->x) / distance * 120,
			(py - props->y) / sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y)) * 120);
		props->timer5 -= H3_GetDeltaTime();
		//if not seen for 5 sec then stop aggro
		if (props->timer4>1) {
			*props->raycast_index += 1;
			snprintf(props->raycasts, 256, "ray_%d", *props->raycast_index);
			props->raycasting = H3_Object_Create(*props->GameScene, props->raycasts, NULL);
			H3_Object_AddComponent(props->raycasting, RAYCASTCOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(props->raycasting, H3_BOX_COLLIDER(CDT_Dynamic, 3, 3, 0x22, true));

			H3_Object_SetTranslation(props->raycasting, props->x, props->y);
			H3_Object_SetVelocity(props->raycasting, (px - props->x) / distance * 1000, (py - props->y) / distance * 1000);
			props->timer4 = 0;
		}
		if (props->timer5 <= 0) {
			props->IsAggro = false;
		}
	}

}


void* EnemyComponent_CreateProperties(H3Handle* player, int* raycast_index, H3Handle* GameScene)
{
	EnemyComponent_Properties* properties = malloc(sizeof(EnemyComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	
	properties->raycasting = NULL;
	properties->GameScene = GameScene;
	properties->raycast_index = raycast_index;
	properties->timer = 0;
	properties->timer2 = 0;
	properties->timer3 = 8;
	properties->timer4 = 0;
	properties->timer5 = 5;
	properties->IsAggro = false;
	properties->raycasting = NULL;
	properties->player = player;
	return properties;
}

void EnemyCollision(H3Handle object, SH3Collision obj_id) {
	SH3Component* component = H3_Object_GetComponent(object, ENEMYCOMPONENT_TYPEID);
	EnemyComponent_Properties* props = (EnemyComponent_Properties*)(component->properties);

}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, bool, IsAggro);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, float, timer5);