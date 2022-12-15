#ifndef PROJECTILECOMPONENT_H_
#define PROJECTILECOMPONENT_H_

#include <h3.h>

#define PROJECTILECOMPONENT_TYPEID 0x1000007

H3_CAPI_BEGIN_BLOCK
void ProjectileComponent_Terminate(void* properties);
void ProjectileComponentPreUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void ProjectileCollisionEnter(H3Handle object, SH3Collision obj_id);

void* ProjectileComponent_CreateProperties(H3Handle player,H3Handle* HitSFX, bool IsMan);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ProjectileComponent, bool, IsLaunched);

H3_CAPI_END_BLOCK

#define PROJECTILECOMPONENT_CREATE(PLAYER,HITSFX, ISMAN)                    \
	(SH3Component) {                                                            \
		.Terminate          = ProjectileComponent_Terminate,                         \
		.PreUpdate          = ProjectileComponentPreUpdate,                             \
		.OnCollisionEnter   = ProjectileCollisionEnter,                              \
		.isInitialized      = false,                                            \
		.componentType      = PROJECTILECOMPONENT_TYPEID,                            \
		.properties         = ProjectileComponent_CreateProperties(PLAYER,HITSFX, ISMAN) \
	}

#endif /* _H3_COMPONENTS_SPRITECOMPONENT_H_ */
