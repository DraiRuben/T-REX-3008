#include "components/Settings.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
	bool* IsSettings;
	bool* IsMainMenu;
	bool* IsMan;
	uint32_t w, h;
	H3Handle BackBtn;
	H3Handle ManSprite;
	H3Handle WomanSprite;
	H3Handle TextureMain;
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
	//background texture
		H3_Texture_Draw(h3, 0, 0, props->TextureMain, A_Top + A_Left);
	//gender select
	if (H3_Button(h3, props->ManSprite, 300, 300, 0x11)) {
		*props->IsMan = true;
	}
	else if (H3_Button(h3, props->WomanSprite, 1200, 300, 0x11)) {
		*props->IsMan = false;
	}
	//back button
	if (H3_Button(h3, props->BackBtn, 800, 700, 0x11)) {
		*props->IsMainMenu = true;
		*props->IsSettings = false;
	
	}
}

void* SettingsComponent_CreateProperties(bool* IsSettings, bool* IsMainMenu, bool* IsMan)
{
	SettingsComponent_Properties* properties = malloc(sizeof(SettingsComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	SettingsComponent_Properties* props = (SettingsComponent_Properties*)properties;

	properties->TextureMain = H3_Texture_Load("assets/Menu/main.jpg", &props->w, &props->h);
	properties->BackBtn = H3_Texture_Load("assets/Menu/Back.png", &props->w, &props->h);
	properties->ManSprite= H3_Texture_Load("assets/Menu/Man.png", &props->w, &props->h);
	properties->WomanSprite = H3_Texture_Load("assets/Menu/Woman.png", &props->w, &props->h);
	properties->IsSettings = IsSettings;
	properties->IsMan = IsMan;
	properties->IsMainMenu = IsMainMenu;
	return properties;
}