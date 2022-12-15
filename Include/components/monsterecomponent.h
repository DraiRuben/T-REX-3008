#ifndef _H3_COMPONENTS_MONSTERECOMPONENT_H_
#define _H3_COMPONENTS_MONSTERECOMPONENT_H_

#include <h3.h>

#define MONSTERECOMPONENT_TYPEID 0x20000004

H3_CAPI_BEGIN_BLOCK
void MonstereComponent_Terminate(void* properties);
void MonstereComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* MonstereComponent_CreateProperties(H3Handle playerRef, H3Handle energyBarRef);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(MonstereComponent, bool, isReadyToUse);
H3_CAPI_END_BLOCK

#define MONSTERECOMPONENT_CREATE(PLAYER, ENERGYBAR)							 \
	(SH3Component) {                                                 \
		.Terminate          = MonstereComponent_Terminate,           \
		.Update             = MonstereComponent_Update,           \
		.isInitialized      = false,                                 \
		.componentType      = MONSTERECOMPONENT_TYPEID,              \
		.properties         = MonstereComponent_CreateProperties(PLAYER, ENERGYBAR)  \
	}

#endif /* _H3_COMPONENTS_MONSTERECOMPONENT_H_ */


