#include "components/Settings.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
	bool* IsSettings;
	bool* IsMainMenu;
	uint32_t w, h;
	H3Handle BackBtn;
} SettingsComponent_Properties;

void SettingsComponent_Terminate(void* properties)
{
	SettingsComponent_Properties* props = (SettingsComponent_Properties*)properties;

	H3_Texture_Destroy(props->BackBtn);
	free(properties);
}

void SettingsComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties)
{
	SettingsComponent_Properties* props = (SettingsComponent_Properties*)properties;
	if (H3_Button(h3, props->BackBtn, 800, 700, 0x11)) {
		*props->IsMainMenu = true;
		*props->IsSettings = false;
	}
}

void* SettingsComponent_CreateProperties(bool* IsSettings, bool* IsMainMenu)
{
	SettingsComponent_Properties* properties = malloc(sizeof(SettingsComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	SettingsComponent_Properties* props = (SettingsComponent_Properties*)properties;

	properties->BackBtn = H3_Texture_Load("assets/Back.png", &props->w, &props->h);
	properties->IsSettings = IsSettings;
	properties->IsMainMenu = IsMainMenu;
	return properties;
}