#ifndef _H3_COMPONENTS_TIREDNESSCOMPONENT_H_
#define _H3_COMPONENTS_TIREDNESSCOMPONENT_H_

#include <h3.h>

#define TIREDNESSCOMPONENT_TYPEID 0x20000001

H3_CAPI_BEGIN_BLOCK
void TirednessComponent_Terminate(void* properties);
void TirednessComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);
void TirednessComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* TirednessComponent_CreateProperties(H3Handle textureBar, H3Handle textureBackBar);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(TirednessSourceComponent, float, energy);
H3_CAPI_END_BLOCK

#define TIREDNESSCOMPONENT_CREATE(TEXTURE, TEXTUREBACK)                                 \
	(SH3Component) {                                                \
		.Terminate          = TirednessComponent_Terminate,         \
		.Draw               = TirednessComponent_Draw,              \
		.Update               = TirednessComponent_Update,              \
		.isInitialized      = false,                                \
		.componentType      = TIREDNESSCOMPONENT_TYPEID,            \
		.properties         = TirednessComponent_CreateProperties(TEXTURE, TEXTUREBACK) \
	}

#endif /* _H3_COMPONENTS_TIREDNESSCOMPONENT_H_ */

