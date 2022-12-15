#ifndef _H3_COMPONENTS_WOMANCOMPONENT_H_
#define _H3_COMPONENTS_WOMANCOMPONENT_H_

#include <h3.h>

#define WOMANCOMPONENT_TYPEID 0x30000002

H3_CAPI_BEGIN_BLOCK
void WomanComponent_Terminate(void* properties);
void WomanComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* WomanComponent_CreateProperties(bool* isWin, bool* isEndGame, bool* isInGame, H3Handle energyBarRef);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(WomanComponent, bool, IsSprint);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(WomanComponent, float, slowdown);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(WomanComponent, bool, playerWin);
H3_CAPI_END_BLOCK

#define WOMANCOMPONENT_CREATE(ADRS_ISWIN, ADRS_ISEND, ADRS_ISGAME, ENERGYBAR)   /* ADRS = address*/                              \
	(SH3Component) {                                                 \
		.Terminate          = WomanComponent_Terminate,             \
		.Update             = WomanComponent_Update,                \
		.isInitialized      = false,                                 \
		.componentType      = WOMANCOMPONENT_TYPEID,                \
		.properties         = WomanComponent_CreateProperties(ADRS_ISWIN, ADRS_ISEND, ADRS_ISGAME, ENERGYBAR) \
	}

#endif /* _H3_COMPONENTS_WOMANCOMPONENT_H_ */