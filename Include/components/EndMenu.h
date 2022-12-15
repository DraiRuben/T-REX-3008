#ifndef ENDMENUCOMPONENT_H_
#define ENDMENUCOMPONENT_H_

#include <h3.h>

#define ENDMENUCOMPONENT_TYPEID 0x30000003

H3_CAPI_BEGIN_BLOCK
void EndMenuComponent_Terminate(void* properties);
void EndMenuComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);

void* EndMenuComponent_CreateProperties(bool* IsEndMenu, bool* IsNewGame, bool* IsMainMenu, bool* IsWin);
H3_CAPI_END_BLOCK
#define ENDMENUCOMPONENT_CREATE(ISENDMENU, ISNEWGAME, ISMAINMENU, ISWIN)                                          \
	(SH3Component) {                                                            \
		.Terminate          = EndMenuComponent_Terminate,                                 \
		.Draw				= EndMenuComponent_Draw,				\
		.isInitialized      = false,                                                     \
		.componentType      = ENDMENUCOMPONENT_TYPEID,                                    \
		.properties         = EndMenuComponent_CreateProperties(ISENDMENU, ISNEWGAME, ISMAINMENU, ISWIN) \
	}

#endif /* _H3_COMPONENTS_ENDMENUCOMPONENT_H_ */