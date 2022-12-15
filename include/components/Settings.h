#ifndef SETTINGSCOMPONENT_H_
#define SETTINGSCOMPONENT_H_

#include <h3.h>

#define SETTINGSCOMPONENT_TYPEID 0x10000003

H3_CAPI_BEGIN_BLOCK
void SettingsComponent_Terminate(void* properties);
void SettingsComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);

void* SettingsComponent_CreateProperties(bool* IsSettings, bool* IsMainMenu, bool* IsMan);
H3_CAPI_END_BLOCK
#define SETTINGSCOMPONENT_CREATE(ISSETTINGS,ISMAINMENU,ISMAN)                                          \
	(SH3Component) {                                                            \
		.Terminate          = SettingsComponent_Terminate,                                 \
		.Draw				= SettingsComponent_Draw,				\
		.isInitialized      = false,                                                     \
		.componentType      = SETTINGSCOMPONENT_TYPEID,                                    \
		.properties         = SettingsComponent_CreateProperties(ISSETTINGS,ISMAINMENU,ISMAN) \
	}

#endif /* _H3_COMPONENTS_SETTINGSCOMPONENT_H_ */
