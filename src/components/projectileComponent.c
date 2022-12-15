#include "components/ProjectileComponent.h"
#include "components/playercomponent.h"
#include "components/projectileComponent.h"
#include "components/collectablecomponent.h"
#include "components/inventorycomponent.h"
#include "components/inventorygirlcomponent.h"
#include "components/EnemyComponent.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
	bool IsMan;
	bool IsLaunched;
	H3Handle player;
	H3Handle ThrowSFX;
	H3Handle* HitSFX;
} ProjectileComponent_Properties;


void ProjectileComponent_Terminate(void* properties)
{
	free(properties);
}
//player pos in map
float px, py;
int mx, my;
//distance between mouse and player for vector normalization
float distance;

void ProjectileComponentPreUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	ProjectileComponent_Properties* props = (ProjectileComponent_Properties*)properties;
	//tracks player and enemy position
	H3_Input_GetMousePos(h3, &mx, &my);
	H3_Transform_GetPosition(H3_Object_GetTransform(props->player), &px, &py);
	//computes distance between player and enemy for vector normalization
	distance = sqrtf((mx/4 - 240) * (mx/4 - 240) + (my/4 - 135) * (my/4 - 135));
	//launches thing
	if (H3_Input_IsMouseBtnPressed(MB_Left) && CollectableComponent_GetisInHandEx(object)) {
		H3_Object_SetRenderOrder(object, 3);
		props->IsLaunched = true;
		H3_Sound_Play(props->ThrowSFX, 0.5, false);
		H3_Object_SetTranslation(object, px, py);
		H3_Object_SetVelocity(object, (mx/4 - 240) / distance*500, (my/4 - 135) / distance*500);
		
		if (props->IsMan)
			InventoryComponent_SetObjSlot2Ex(props->player, NULL);
		else
			InventoryGirlComponent_SetObjSlot3Ex(props->player, NULL);

		CollectableComponent_SetisInHandEx(object, false);
	}
	if (props->IsLaunched) {
		H3_Object_Rotate(object, 500 * H3_GetDeltaTime());
	}
}


void* ProjectileComponent_CreateProperties(H3Handle player,H3Handle* HitSFX, bool IsMan)
{
	ProjectileComponent_Properties* properties = malloc(sizeof(ProjectileComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->IsMan = IsMan;
	properties->player = player;
	properties->IsLaunched = false;
	properties->HitSFX = HitSFX;
	properties->ThrowSFX = H3_Sound_Load("assets/SFX/ThrowSFX.wav");

	return properties;
}
float vx, vy;

void ProjectileCollisionEnter(H3Handle object, SH3Collision obj_id) {
	SH3Component* component = H3_Object_GetComponent(object, PROJECTILECOMPONENT_TYPEID);
	ProjectileComponent_Properties* props = (ProjectileComponent_Properties*)(component->properties);
	
	if (props->IsLaunched && obj_id.other == NULL) {
		H3_Sound_Play(*props->HitSFX, 0.4, false);
		CollectableComponent_SetdurabilityEx(object, CollectableComponent_GetdurabilityEx(object) - 1);
	}
	else if (props->IsLaunched && H3_Object_HasComponent(obj_id.other, ENEMYCOMPONENT_TYPEID)) {
		H3_Sound_Play(*props->HitSFX, 0.4, false);
		H3_Object_GetVelocity(object, &vx, &vy);
		EnemyComponent_SetIsStunnedEx(obj_id.other, true);
		H3_Object_SetVelocity(obj_id.other, vx/2, vy /2);
		CollectableComponent_SetdurabilityEx(object, CollectableComponent_GetdurabilityEx(object) - 1);
	}
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ProjectileComponent, PROJECTILECOMPONENT_TYPEID, bool, IsLaunched);