#include "components/Credits.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
	bool* IsCredits;
	bool* IsMainMenu;
	uint32_t w, h;
	H3Handle BackBtn;
} CreditsComponent_Properties;

void CreditsComponent_Terminate(void* properties)
{
	CreditsComponent_Properties* props = (CreditsComponent_Properties*)properties;

	H3_Texture_Destroy(props->BackBtn);
	free(properties);
}

void CreditsComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties)
{
	CreditsComponent_Properties* props = (CreditsComponent_Properties*)properties;
	if (H3_Button(h3, props->BackBtn, 800, 700, 0x11)) {
		*props->IsMainMenu = true;
		*props->IsCredits = false;
	}
}

void* CreditsComponent_CreateProperties(bool* IsCredits,bool* IsMainMenu)
{
	CreditsComponent_Properties* properties = malloc(sizeof(CreditsComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	CreditsComponent_Properties* props = (CreditsComponent_Properties*)properties;

	properties->BackBtn = H3_Texture_Load("assets/Menu/Back.png", &props->w, &props->h);
	properties->IsCredits = IsCredits;
	properties->IsMainMenu = IsMainMenu;
	return properties;
}