#include "components/EnemyComponent.h"
#include "components/playercomponent.h"
#include "components/tirednesscomponent.h"
#include "components/spritecomponent.h"
#include "components/animatedspritecomponent.h"
#include "components/raycastcomponent.h"
#include "components/SpawnerComponent.h"
#include "components/ClockComponent.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
	//useful bools
	bool IsStunned;
	bool* IsNewWave;
	bool IsAggro;
	bool ResetIndexes;
	bool isTouchPlayer;
	bool* GlobalAggro;
	bool IsTemp;

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
	float StunTimer;
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
	H3Handle energyBar;

	//texture animation
	uint32_t TxW, TxH;
	H3Handle TxRunDown;
	H3Handle TxRunUp;
	H3Handle TxRunLeft;
	H3Handle TxRunRight;
	H3Handle TxIdleDown;
	//sfx
	H3Handle* SpottedSFX;
	bool SpottedSFXonce;
} EnemyComponent_Properties;


void EnemyComponent_Terminate(void* properties)
{
	free(properties);
}

//player pos in map
float px, py;
//distance between player and enemy for vector normalization
float distance;
float distancetemp;
float globalAggroOff = 0;

void EnemyComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	EnemyComponent_Properties* props = (EnemyComponent_Properties*)properties;
	//tracks player and enemy position
	H3_Transform_GetPosition(transform, &props->x, &props->y);
	H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &px, &py);
	//computes distance between player and enemy for vector normalization
	distance = sqrtf((px - props->x) * (px - props->x) + (py - props->y) * (py - props->y));


	//idle
	if (props->IsAggro == false) {
		props->SpottedSFXonce = false;
		//chooses a random direction every 7 sec
		props->DirectionTimer += H3_GetDeltaTime();
		if (props->DirectionTimer > 7) {
			props->direction = ((rand() % 4) + 1);
			props->DirectionTimer = 0;
		}
		//move periodically in choosen direction
		props->IdleMoveTimer += H3_GetDeltaTime();
		if (props->IdleMoveTimer > 2&& !props->IsStunned) {
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
		//raycast to check if the bot can see the player
		props->RaycastTimer += H3_GetDeltaTime();
		if (distance < 200 && props->RaycastTimer>0.2) {
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
		if (!props->SpottedSFXonce) {
			H3_Sound_Play(*props->SpottedSFX, 0.6, false);
			props->SpottedSFXonce = true;
		}
		props->FollowTimer += H3_GetDeltaTime();
		//used by raycast so the bot doesn't do the same path as player if the player can be seen by the bot
		if (props->ResetIndexes) {
			props->index = 0;
			props->index2 = 0;
			props->ResetIndexes = false;
		}
		//tracks player position in list every 0.2 sec
		H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &px, &py);
		if (props->FollowTimer > 0.2) {
			props->px[props->index] = px;
			props->py[props->index] = py;
			props->index += 1;
			props->FollowTimer = 0;
		}
		
		
		//get player and self pos
		H3_Transform_GetPosition(transform, &props->x, &props->y);
		H3_Transform_GetPosition(H3_Object_GetTransform(*props->player), &px, &py);
		//vector normalisation for movement with list
		distancetemp = sqrtf((props->px[props->index2] - props->x) * (props->px[props->index2] - props->x) + (props->py[props->index2] - props->y) * (props->py[props->index2] - props->y));
		//go to player position in list if not stunned and not touching player as to not push him
		if (!props->isTouchPlayer && !props->IsStunned) {
			H3_Object_SetVelocity(object, (props->px[props->index2] - props->x) / distancetemp * 110, (props->py[props->index2] - props->y) / distancetemp * 110);
		}
		//go to next pos in list if close enough to current pos in list
		if (fabs( props->px[props->index2]-props->x ) < 10 && fabs(props->py[props->index2]-props->y) < 10) {
			props->index2 += 1;
		}
		//if not seen for 5 sec (15s if during wave) then stop aggro
		props->AggroTimer -= H3_GetDeltaTime();
		if (props->RaycastTimer > 0.2&& !props->IsStunned) {
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
			if (props->IsTemp) {
				if (fabs(px - props->x) > 240 && fabs(py - props->y) > 135) {
					H3_Object_Destroy(object, false);
				}
			}
		}

		//raycast check obstacle each second to reset aggrotimer if needed, MAY BE REPETITIVE
		props->RaycastTimer += H3_GetDeltaTime();
		if (distance < 200 && props->RaycastTimer>0.2&& !props->IsStunned) {
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

	//increases tiredness when touching player
	if (props->isTouchPlayer)
	{
		float tiredness = TirednessComponent_GettirednessEx(props->energyBar);
		tiredness += 0.15f * H3_GetDeltaTime(); //fills 15%/s
		TirednessComponent_SettirednessEx(props->energyBar, tiredness);
	}

	//for wave every hour
	if (*props->GlobalAggro) {
		props->AggroTimer = 15;
		props->IsAggro = true;
		globalAggroOff += H3_GetDeltaTime();
		if (globalAggroOff > 1) {
			*props->GlobalAggro = false;
			globalAggroOff = 0;
		}
	}

	//for projectile and melee stun
	if (props->IsStunned) {
		props->StunTimer += H3_GetDeltaTime();
		if (props->StunTimer > 0.5) {
			H3_Object_SetVelocity(object, 0, 0);
		}
		if (props->StunTimer > 3) {
			props->IsStunned = false;
			props->StunTimer = 0;
		}
	}

	//animation based on current velocity, works well except when player pushes the enemies
	float vx, vy;
	H3_Object_GetVelocity(object, &vx, &vy);

	if (vx > vy && vx > -vy) //run
		AnimatedSpriteComponent_SetTextureEx(object, props->TxRunRight);
	else if (vx < vy && vx < -vy)
		AnimatedSpriteComponent_SetTextureEx(object, props->TxRunLeft);
	else if (vy > vx && vy > -vx)
		AnimatedSpriteComponent_SetTextureEx(object, props->TxRunDown);
	else if (vy < vx && vy < -vx)
		AnimatedSpriteComponent_SetTextureEx(object, props->TxRunUp);

	if (vx == 0 && vy == 0)	//Idle
		AnimatedSpriteComponent_SetTextureEx(object, props->TxIdleDown);
}


void* EnemyComponent_CreateProperties(H3Handle* player, int* raycast_index, H3Handle* GameScene, H3Handle energyBarRef,bool* IsNewWave,bool* GlobalAggro,bool IsTemp,H3Handle* SpottedSFX)
{
	EnemyComponent_Properties* properties = malloc(sizeof(EnemyComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->GlobalAggro = GlobalAggro;
	properties->IsTemp = IsTemp;
	properties->energyBar = energyBarRef;
	properties->isTouchPlayer = false;
	properties->IsNewWave = IsNewWave;
	properties->raycasting = NULL;
	properties->GameScene = GameScene;
	properties->raycast_index = raycast_index;
	properties->IdleMoveTimer = 0;
	properties->timer2 = 0;
	properties->DirectionTimer = 8;
	properties->RaycastTimer = 0;
	properties->AggroTimer = 5;
	properties->FollowTimer = 8;
	properties->StunTimer = 0;
	properties->index = 0;
	properties->index2 = 0;
	properties->IsAggro = false;
	properties->ResetIndexes = false;
	properties->raycasting = NULL;
	properties->player = player;

	//texture animation
	properties->TxRunDown	= H3_Texture_Load("assets/Sprites/Enemy/EnemyMoveRun.png", &properties->TxW, &properties->TxH);
	properties->TxRunUp		= H3_Texture_Load("assets/Sprites/Enemy/EnemyRunUp.png", &properties->TxW, &properties->TxH);
	properties->TxRunLeft	= H3_Texture_Load("assets/Sprites/Enemy/EnemyRunLeft.png", &properties->TxW, &properties->TxH);
	properties->TxRunRight	= H3_Texture_Load("assets/Sprites/Enemy/EnemyRunRight.png", &properties->TxW, &properties->TxH);
	properties->TxIdleDown	= H3_Texture_Load("assets/Sprites/Enemy/EnemyIdleDown.png", &properties->TxW, &properties->TxH);

	//sfx
	properties->SpottedSFX = SpottedSFX;
	properties->SpottedSFXonce = false;
	return properties;
}

//detect player collision
void EnemyCollisionEnter(H3Handle object, SH3Collision obj_id) {
	SH3Component* component = H3_Object_GetComponent(object, ENEMYCOMPONENT_TYPEID);
	EnemyComponent_Properties* props = (EnemyComponent_Properties*)(component->properties);

	if (obj_id.other != NULL && H3_Object_HasComponent(obj_id.other, PLAYERCOMPONENT_TYPEID))
	{
		props->isTouchPlayer = true;
	}
}

void EnemyCollisionLeave(H3Handle object, H3Handle other) {
	SH3Component* component = H3_Object_GetComponent(object, ENEMYCOMPONENT_TYPEID);
	EnemyComponent_Properties* props = (EnemyComponent_Properties*)(component->properties);

	if (other != NULL && H3_Object_HasComponent(other, PLAYERCOMPONENT_TYPEID))
	{
		props->isTouchPlayer = false;
	}
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, bool, IsTemp);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, bool, IsAggro);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, bool, IsStunned);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, bool, ResetIndexes);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, float, AggroTimer);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnemyComponent, ENEMYCOMPONENT_TYPEID, float, FollowTimer);