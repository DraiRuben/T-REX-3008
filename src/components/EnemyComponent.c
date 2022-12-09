#include "components/EnemyComponent.h"
#include "components/spritecomponent.h"
#include "components/raycastcomponent.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
	bool IsAggro;

	//bot pos
	float x, y;

	//random idle direction
	int direction;

	//tracks player coordinates during pursuit
	float px[3000];
	float py[3000];

	//timers
	float IdleMoveTimer;
	float timer2;
	float DirectionTimer;
	float RaycastTimer;
	float AggroTimer;
	float FollowTimer;

	//indexes to go through lists
	int index;
	int index2;

	//raycast
	int* raycast_index;
	char raycasts[256];
	H3Handle raycasting;

	//ref to player and scene
	H3Handle* player;
	H3Handle* GameScene;

} EnemyComponent_Properties;


void EnemyComponent_Terminate(void* properties)
{
	free(properties);
}
//player pos in map
float px, py;
//distance between player and enemy for vector normalization
float distance;


void EnemyComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties){
EnemyComponent_Properties* props = (EnemyComponent_Properties*)properties;
	//tracks player and enemy position
	H3_Transform_GetPosition(transform, &props->x, &props->y);
	H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &px, &py);
	//computes distance between player and enemy for vector normalization
	distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));

	
	//idle
	if (props->IsAggro == false) {
		

		//chooses a random direction every 7 sec
		props->DirectionTimer += H3_GetDeltaTime();
		if (props->DirectionTimer > 7) {
			props->direction = ((rand() % 4) + 1);
			props->DirectionTimer = 0;
		}
		//move periodically in choosen direction
		props->IdleMoveTimer += H3_GetDeltaTime();
		if (props->IdleMoveTimer > 2) {
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
			//stops moving after 3 sec
			props->timer2 += H3_GetDeltaTime();
			if (props->timer2 > 3) {
				H3_Object_SetVelocity(object, 0, 0);
				props->timer2 = 0;
				props->IdleMoveTimer = 0;
			}
		}
		//raycast check obstacle at certain disance between player and bot each second
		props->RaycastTimer += H3_GetDeltaTime();
		if (distance < 200&&props->RaycastTimer>1) {
			*props->raycast_index += 1;
			snprintf(props->raycasts, 256, "ray_%d", *props->raycast_index);
			props->raycasting = H3_Object_Create(*props->GameScene, props->raycasts, NULL);
			H3_Object_AddComponent(props->raycasting, RAYCASTCOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(props->raycasting, H3_BOX_COLLIDER(CDT_Dynamic, 3, 3, 0x22, true));
			H3_Object_SetTranslation(props->raycasting, props->x, props->y);
			H3_Object_SetVelocity(props->raycasting, (px - props->x) / distance * 1000, (py - props->y) / distance * 1000);
			props->RaycastTimer = 0;
		}
	}

	//aggro'd
	if (props->IsAggro == true) {
		//go to player position once to start loop
		props->FollowTimer += H3_GetDeltaTime();
		if (props->FollowTimer >= 7) {
			H3_Object_SetVelocity(object,(px-props->x)/distance*500,(py-props->y)/distance*500 );
		}
		//tracks player position in list every 0.2 sec
		if (props->FollowTimer > 0.2) {
			props->px[props->index] = px;
			props->py[props->index] = py;
			props->index += 1;
			props->FollowTimer = 0;
			
		}
		//go to player position in list
		H3_Object_SetVelocity(object, (props->px[props->index2] - props->x) / sqrtf((props->px[props->index2] - props->x) * (props->px[props->index2] - props->x) + (props->py[props->index2] - props->y) * (props->py[props->index2] - props->y)) * 150,
			(props->py[props->index2] - props->y) / sqrtf((props->px[props->index2] - props->x) * (props->px[props->index2] - props->x) + (props->py[props->index2] - props->y) * (props->py[props->index2] - props->y)) * 150);
		//if close enough to player pos in list, go to the next one
		if (fabs(props->x - props->px[props->index2])<10&& fabs(props->y - props->py[props->index2])<10) {
			props->index2 += 1;
		}
		
		//if not seen for 5 sec then stop aggro
		props->AggroTimer -= H3_GetDeltaTime();
		if (props->RaycastTimer>1) {
			*props->raycast_index += 1;
			snprintf(props->raycasts, 256, "ray_%d", *props->raycast_index);
			props->raycasting = H3_Object_Create(*props->GameScene, props->raycasts, NULL);
			H3_Object_AddComponent(props->raycasting, RAYCASTCOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(props->raycasting, H3_BOX_COLLIDER(CDT_Dynamic, 3, 3, 0x22, true));

			H3_Object_SetTranslation(props->raycasting, props->x, props->y);
			H3_Object_SetVelocity(props->raycasting, (px - props->x) / distance * 1000, (py - props->y) / distance * 1000);
			props->RaycastTimer = 0;
		}
		if (props->AggroTimer <= 0) {
			props->index2 = 0;
			props->index = 0;
			props->IsAggro = false;
		}
		//raycast check obstacle each second to reset aggrotimer if needed
		props->RaycastTimer += H3_GetDeltaTime();
		if (distance < 200 && props->RaycastTimer>1) {
			*props->raycast_index += 1;
			snprintf(props->raycasts, 256, "ray_%d", *props->raycast_index);
			props->raycasting = H3_Object_Create(*props->GameScene, props->raycasts, NULL);
			H3_Object_AddComponent(props->raycasting, RAYCASTCOMPONENT_CREATE(object));
			H3_Object_EnablePhysics(props->raycasting, H3_BOX_COLLIDER(CDT_Dynamic, 3, 3, 0x22, true));
			H3_Object_SetTranslation(props->raycasting, props->x, props->y);
			H3_Object_SetVelocity(props->raycasting, (px - props->x) / distance * 1000, (py - props->y) / distance * 1000);
			props->RaycastTimer = 0;
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
	properties->IdleMoveTimer = 0;
	properties->timer2 = 0;
	properties->DirectionTimer = 8;
	properties->RaycastTimer = 0;
	properties->AggroTimer = 5;
	properties->FollowTimer = 8;
	properties->index = 0;
	properties->index2 = 0;
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
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, float, AggroTimer);