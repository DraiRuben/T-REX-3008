#include "components/MainMenu.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct
{
	bool* IsMainMenu;
	bool* IsCredits;
	bool* IsSettings;
	bool* IsNewGame;
	uint32_t w, h;
	H3Handle NewGameBtn;
	H3Handle CreditsBtn;
	H3Handle SettingsBtn;
	H3Handle ExitBtn;
} MainMenuComponent_Properties;

void MainMenuComponent_Terminate(void* properties)
{
	MainMenuComponent_Properties* props = (MainMenuComponent_Properties*)properties;

	H3_Texture_Destroy(props->NewGameBtn);
	H3_Texture_Destroy(props->CreditsBtn);
	H3_Texture_Destroy(props->SettingsBtn);
	H3_Texture_Destroy(props->ExitBtn);
	free(properties);
}

void MainMenuComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties)
{
	MainMenuComponent_Properties* props = (MainMenuComponent_Properties*)properties;
	if (H3_Button(h3, props->NewGameBtn, 500, 200, 0x11)) {
		*props->IsMainMenu = false;
		*props->IsNewGame = true;
	}
	if (H3_Button(h3, props->CreditsBtn, 500, 300, 0x11)) {
		*props->IsMainMenu = false;
		*props->IsCredits = true;
	}
	if (H3_Button(h3, props->SettingsBtn, 500, 400, 0x11)) {
		*props->IsMainMenu = false;
		*props->IsSettings = true;
	}
	if (H3_Button(h3, props->ExitBtn, 500, 500, 0x11)) {
		exit(0);
		
	}
}

void* MainMenuComponent_CreateProperties(bool* IsMainMenu,bool* IsCredits,bool* IsSettings, bool* IsNewGame)
{
	MainMenuComponent_Properties* properties = malloc(sizeof(MainMenuComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	MainMenuComponent_Properties* props = (MainMenuComponent_Properties*)properties;
	properties->NewGameBtn = H3_Texture_Load("assets/NewGame.png", &props->w, &props->h);
	properties->CreditsBtn = H3_Texture_Load("assets/Credits.png", &props->w, &props->h);
	properties->SettingsBtn = H3_Texture_Load("assets/Settings.png", &props->w, &props->h);
	properties->ExitBtn = H3_Texture_Load("assets/Exit.png", &props->w, &props->h);
	properties->IsMainMenu = IsMainMenu;
	properties->IsCredits = IsCredits;
	properties->IsSettings = IsSettings;
	properties->IsNewGame = IsNewGame;
	return properties;
}