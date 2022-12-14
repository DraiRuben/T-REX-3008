#ifndef _H3_COMPONENTS_PLAYERCOMPONENT_H_
#define _H3_COMPONENTS_PLAYERCOMPONENT_H_

#include <h3.h>

#define PLAYERCOMPONENT_TYPEID 0x30000002

H3_CAPI_BEGIN_BLOCK
void PlayerComponent_Terminate(void* properties);
void PlayerComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* PlayerComponent_CreateProperties(bool* isWin, bool* isEndGame, bool* isInGame, H3Handle energyBarRef);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(PlayerComponent, bool, IsSprint);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(PlayerComponent, float, slowdown);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(PlayerComponent, bool, playerWin);
H3_CAPI_END_BLOCK

#define PLAYERCOMPONENT_CREATE(ADRS_ISWIN, ADRS_ISEND, ADRS_ISGAME, ENERGYBAR)   /* ADRS = address*/                              \
	(SH3Component) {                                                 \
		.Terminate          = PlayerComponent_Terminate,             \
		.Update             = PlayerComponent_Update,                \
		.isInitialized      = false,                                 \
		.componentType      = PLAYERCOMPONENT_TYPEID,                \
		.properties         = PlayerComponent_CreateProperties(ADRS_ISWIN, ADRS_ISEND, ADRS_ISGAME, ENERGYBAR) \
	}

#endif /* _H3_COMPONENTS_PLAYERCOMPONENT_H_ */