#include "components/RaycastComponent.h"
#include "components/EnemyComponent.h"
#include "components/playercomponent.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
	float timer;
	H3Handle Owner;
} RaycastComponent_Properties;


void RaycastComponent_Terminate(void* properties)
{
	free(properties);
}

void RaycastComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	RaycastComponent_Properties* props = (RaycastComponent_Properties*)properties;
	props->timer += H3_GetDeltaTime();
	if (props->timer > 3) {
		H3_Object_Destroy(object,false);
	}

}


void* RaycastComponent_CreateProperties(H3Handle Owner)
{
	RaycastComponent_Properties* properties = malloc(sizeof(RaycastComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->timer = 0;
	properties->Owner = Owner;
	return properties;
}

void RaycastComponentCollision(H3Handle object, SH3Collision obj_id) {
	SH3Component* component = H3_Object_GetComponent(object, RAYCASTCOMPONENT_TYPEID);
	RaycastComponent_Properties* props = (RaycastComponent_Properties*)(component->properties);
	if (obj_id.other == NULL) {
		H3_Object_Destroy(object, false);
	}
	else if (H3_Object_HasComponent(obj_id.other, PLAYERCOMPONENT_TYPEID)) {
		EnemyComponent_SetIsAggroEx(props->Owner, true);
		H3_Object_Destroy(object, false);
		EnemyComponent_Settimer5Ex(props->Owner, 5);
	}
}


