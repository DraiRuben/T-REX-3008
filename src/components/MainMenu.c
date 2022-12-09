#include "components/MainMenu.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct
{
	//required bools to track gamestate
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
	//launch game
	if (H3_Button(h3, props->NewGameBtn,800, 400, 0x11)) {
		*props->IsMainMenu = false;
		*props->IsNewGame = true;
	}
	//show credits
	if (H3_Button(h3, props->CreditsBtn, 800, 500, 0x11)) {
		*props->IsMainMenu = false;
		*props->IsCredits = true;
	}
	//go to settings
	if (H3_Button(h3, props->SettingsBtn, 800, 600, 0x11)) {
		*props->IsMainMenu = false;
		*props->IsSettings = true;
	}
	//close game
	if (H3_Button(h3, props->ExitBtn, 800, 700, 0x11)) {
		exit(0);
	}
}

void* MainMenuComponent_CreateProperties(bool* IsMainMenu,bool* IsCredits,bool* IsSettings, bool* IsNewGame)
{
	MainMenuComponent_Properties* properties = malloc(sizeof(MainMenuComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	MainMenuComponent_Properties* props = (MainMenuComponent_Properties*)properties;
	//load needed btn images
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