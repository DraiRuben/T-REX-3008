#ifndef AISLESPAWNERCOMPONENT_H_
#define AISLESPAWNERCOMPONENT_H_

#include <h3.h>

#define AISLESPAWNERCOMPONENT_TYPEID 0x10000008

H3_CAPI_BEGIN_BLOCK
void AisleSpawnerComponent_Terminate(void* properties);
void AisleSpawnerComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* AisleSpawnerComponent_CreateProperties(H3Handle* GameScene);

H3_CAPI_END_BLOCK

#define AISLESPAWNERCOMPONENT_CREATE(GAMESCENE)                                          \
	(SH3Component) {                                                            \
		.Terminate          = AisleSpawnerComponent_Terminate,                                 \
		.Update             = AisleSpawnerComponentUpdate,                  \
		.isInitialized      = false,             \
		.componentType      = AISLESPAWNERCOMPONENT_TYPEID,                                    \
		.properties         = AisleSpawnerComponent_CreateProperties(GAMESCENE) \
	}

#endif /* _H3_COMPONENTS_SPRITECOMPONENT_H_ */
