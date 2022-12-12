#ifndef SPAWNERCOMPONENT_H_
#define SPAWNERCOMPONENT_H_

#include <h3.h>

#define SPAWNERCOMPONENT_TYPEID 0x10000007

H3_CAPI_BEGIN_BLOCK
void SpawnerComponent_Terminate(void* properties);
void SpawnerComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void SpawnerCollision(H3Handle object, SH3Collision obj_id);

void* SpawnerComponent_CreateProperties(H3Handle* player,H3Handle* GameScene, H3Handle energyBar, bool* IsNewWave, bool* IsWave,bool* GlobalAggro );


H3_CAPI_END_BLOCK

#define SPAWNERCOMPONENT_CREATE(PLAYER,GAMESCENE,ENERGYBAR,ISNEWWAVE,ISWAVE,GLOBALAGGRO)                                          \
	(SH3Component) {                                                            \
		.Terminate          = SpawnerComponent_Terminate,                                 \
		.Update             = SpawnerComponentUpdate,                  \
		.OnCollisionEnter   = SpawnerCollision,   \
		.isInitialized      = false,             \
		.componentType      = SPAWNERCOMPONENT_TYPEID,                                    \
		.properties         = SpawnerComponent_CreateProperties(PLAYER,GAMESCENE,ENERGYBAR,ISNEWWAVE,ISWAVE,GLOBALAGGRO) \
	}

#endif /* _H3_COMPONENTS_SPRITECOMPONENT_H_ */
