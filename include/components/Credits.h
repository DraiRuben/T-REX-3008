#ifndef CREDITSCOMPONENT_H_
#define CREDITSCOMPONENT_H_

#include <h3.h>

#define MAINMENUCOMPONENT_TYPEID 0x10000002

H3_CAPI_BEGIN_BLOCK
void CreditsComponent_Terminate(void* properties);
void CreditsComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);

void* CreditsComponent_CreateProperties(bool* IsCredits,bool* IsMainMenu);
H3_CAPI_END_BLOCK
#define CREDITSCOMPONENT_CREATE(ISCREDITS,ISMAINMENU)                                          \
	(SH3Component) {                                                            \
		.Terminate          = CreditsComponent_Terminate,                                 \
		.Draw				= CreditsComponent_Draw,				\
		.isInitialized      = false,                                                     \
		.componentType      = MAINMENUCOMPONENT_TYPEID,                                    \
		.properties         = CreditsComponent_CreateProperties(ISCREDITS,ISMAINMENU) \
	}

#endif /* _H3_COMPONENTS_CREDITSCOMPONENT_H_ */
