#ifndef CLOCKCOMPONENT_H_
#define CLOCKCOMPONENT_H_

#include <h3.h>

#define CLOCKCOMPONENT_TYPEID 0x10000004

H3_CAPI_BEGIN_BLOCK
void ClockComponent_Terminate(void* properties);
void ClockComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);

void* ClockComponent_CreateProperties(SH3TextProperties* textprops,bool* IsNewWave);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(ClockComponent, char, time);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ClockComponent, float, hours);

H3_CAPI_END_BLOCK
#define CLOCKCOMPONENT_CREATE(TEXTPROPS,ISNEWWAVE)                                          \
	(SH3Component) {                                                            \
		.Terminate          = ClockComponent_Terminate,                                 \
		.Update				= ClockComponent_Update,				\
		.isInitialized      = false,                                                     \
		.componentType      = CLOCKCOMPONENT_TYPEID,                                    \
		.properties         = ClockComponent_CreateProperties(TEXTPROPS,ISNEWWAVE) \
	}

#endif /* _H3_COMPONENTS_CLOCKCOMPONENT_H_ */
