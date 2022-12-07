#ifndef MAINMENUCOMPONENT_H_
#define MAINMENUCOMPONENT_H_

#include <h3.h>

#define MAINMENUCOMPONENT_TYPEID 0x10000001

H3_CAPI_BEGIN_BLOCK
void MainMenuComponent_Terminate(void* properties);
void MainMenuComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);

void* MainMenuComponent_CreateProperties(bool* IsMainMenu,bool* IsCredits,bool* IsSettings,bool* IsNewGame);
H3_CAPI_END_BLOCK
#define MAINMENUCOMPONENT_CREATE(ISMAINMENU,ISCREDITS,ISSETTINGS,ISNEWGAME)                                          \
	(SH3Component) {                                                            \
		.Terminate          = MainMenuComponent_Terminate,                                 \
		.Draw				= MainMenuComponent_Draw,				\
		.isInitialized      = false,                                                     \
		.componentType      = MAINMENUCOMPONENT_TYPEID,                                    \
		.properties         = MainMenuComponent_CreateProperties(ISMAINMENU,ISCREDITS,ISSETTINGS,ISNEWGAME) \
	}

#endif /* _H3_COMPONENTS_MAINMENUCOMPONENT_H_ */
