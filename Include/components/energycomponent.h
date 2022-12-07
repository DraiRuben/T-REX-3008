#ifndef _H3_COMPONENTS_ENERGYCOMPONENT_H_
#define _H3_COMPONENTS_ENERGYCOMPONENT_H_

#include <h3.h>

#define ENERGYCOMPONENT_TYPEID 0x20000001

H3_CAPI_BEGIN_BLOCK
void EnergyComponent_Terminate(void* properties);
void EnergyComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);
void EnergyComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* EnergyComponent_CreateProperties(H3Handle textureBar, H3Handle textureBackBar);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnergySourceComponent, float, energy);
H3_CAPI_END_BLOCK

#define ENERGYCOMPONENT_CREATE(TEXTURE, TEXTUREBACK)                                 \
	(SH3Component) {                                                \
		.Terminate          = EnergyComponent_Terminate,         \
		.Draw               = EnergyComponent_Draw,              \
		.Update               = EnergyComponent_Update,              \
		.isInitialized      = false,                                \
		.componentType      = ENERGYCOMPONENT_TYPEID,            \
		.properties         = EnergyComponent_CreateProperties(TEXTURE, TEXTUREBACK) \
	}

#endif /* _H3_COMPONENTS_ENERGYCOMPONENT_H_ */

