#ifndef RAYCASTCOMPONENT_H_
#define RAYCASTCOMPONENT_H_

#include <h3.h>

#define RAYCASTCOMPONENT_TYPEID 0x10000006

H3_CAPI_BEGIN_BLOCK
void RaycastComponent_Terminate(void* properties);
void RaycastComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void RaycastComponentCollision(H3Handle object, SH3Collision obj_id);

void* RaycastComponent_CreateProperties(H3Handle Owner);
H3_CAPI_END_BLOCK

#define RAYCASTCOMPONENT_CREATE(OWNER)                                          \
	(SH3Component) {                                                            \
		.Terminate          = RaycastComponent_Terminate,                                 \
		.Update             = RaycastComponentUpdate,                  \
		.OnCollisionEnter	= RaycastComponentCollision, \
		.isInitialized      = false,             \
		.componentType      = RAYCASTCOMPONENT_TYPEID,                                    \
		.properties         = RaycastComponent_CreateProperties(OWNER) \
	}

#endif /* _H3_COMPONENTS_SPRITECOMPONENT_H_ */
