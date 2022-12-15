#ifndef _H3_COMPONENTS_MANCOMPONENT_H_
#define _H3_COMPONENTS_MANCOMPONENT_H_

#include <h3.h>

#define MANCOMPONENT_TYPEID 0x30000002

H3_CAPI_BEGIN_BLOCK
void ManComponent_Terminate(void* properties);
void ManComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* ManComponent_CreateProperties(bool* isWin, bool* isEndGame, bool* isInGame, H3Handle energyBarRef);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(ManComponent, bool, IsSprint);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ManComponent, float, slowdown);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ManComponent, bool, playerWin);
H3_CAPI_END_BLOCK

#define MANCOMPONENT_CREATE(ADRS_ISWIN, ADRS_ISEND, ADRS_ISGAME, ENERGYBAR)   /* ADRS = address*/                              \
	(SH3Component) {                                                 \
		.Terminate          = ManComponent_Terminate,             \
		.Update             = ManComponent_Update,                \
		.isInitialized      = false,                                 \
		.componentType      = MANCOMPONENT_TYPEID,                \
		.properties         = ManComponent_CreateProperties(ADRS_ISWIN, ADRS_ISEND, ADRS_ISGAME, ENERGYBAR) \
	}

#endif /* _H3_COMPONENTS_MANCOMPONENT_H_ */