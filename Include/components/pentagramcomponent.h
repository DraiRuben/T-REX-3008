#ifndef PENTAGRAMCOMPONENT_H_
#define PENTAGRAMCOMPONENT_H_

#include <h3.h>

#define PENTAGRAMCOMPONENT_TYPEID 0x1000008

H3_CAPI_BEGIN_BLOCK
void PentagramComponent_Terminate(void* properties);
void PentagramComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void PentagramTriggerEnter(H3Handle object, SH3Collision obj_id);

void* PentagramComponent_CreateProperties(const char* path);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(PentagramComponent, bool, IsActivated);


H3_CAPI_END_BLOCK

#define PENTAGRAMCOMPONENT_CREATE(TEXTUREPATH)                    \
	(SH3Component) {                                                            \
		.Terminate          = PentagramComponent_Terminate,                         \
		.Update				= PentagramComponentUpdate,                             \
		.OnTriggerEnter     = PentagramTriggerEnter,                              \
		.isInitialized      = false,                                            \
		.componentType      = PENTAGRAMCOMPONENT_TYPEID,                            \
		.properties         = PentagramComponent_CreateProperties(TEXTUREPATH) \
	}

#endif /* _H3_COMPONENTS_SPRITECOMPONENT_H_ */
