#ifndef _H3_COMPONENTS_COLLECTABLECOMPONENT_H_
#define _H3_COMPONENTS_COLLECTABLECOMPONENT_H_

#include <h3.h>

#define COLLECTABLECOMPONENT_TYPEID 0x20000003

H3_CAPI_BEGIN_BLOCK
void CollectableComponent_Terminate(void* properties);
void CollectableComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* CollectableComponent_CreateProperties(int type,int durability);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(CollectableComponent, bool, isInHand);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(CollectableComponent, int, type);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(CollectableComponent, int, durability);

H3_CAPI_END_BLOCK

#define COLLECTABLECOMPONENT_CREATE(TYPE,DURABILITY)									 \
	(SH3Component) {                                                 \
		.Terminate          = CollectableComponent_Terminate,          \
		.Update             = CollectableComponent_Update,             \
		.isInitialized      = false,                                 \
		.componentType      = COLLECTABLECOMPONENT_TYPEID,             \
		.properties         = CollectableComponent_CreateProperties(TYPE,DURABILITY)  \
	}

#endif /* _H3_COMPONENTS_COLLECTABLECOMPONENT_H_ */

