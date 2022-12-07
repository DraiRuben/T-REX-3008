#include <h3.h>
#include "components/MainMenu.h"
#include "components/Credits.h"
#include "components/Settings.h"
#include "components/textcomponent.h"
#include "components/spritecomponent.h"
#ifndef NDEBUG
# pragma comment(lib, "h3-s-d.lib")
#else // !NDEBUG
# pragma comment(lib, "h3-s.lib")
#endif // !NDEBUG

int main()
{
	//Text Init
	SH3TextProperties textprops = (SH3TextProperties){
			.font = H3_Font_Load("assets/Toriko.ttf"),
			.size = 50,
			.fillColor = {.r = 255,.g = 255,.b = 255,.a = 255},
			.hasOutline = false,
			.anchor = 0x22,
			.isBold = true,
			.isItalic = false,
			.isUnderlined = false,
			.isViewLocal = false,
	};
	//Window Init
	H3Handle screen = H3_Init((SH3InitParams) {
			.width = 1920,
			.height = 1080,
			.fullscreen = false,
			.windowTitle = "T-REX3008"
	});
	bool IsMainMenu = true;
	bool IsCredits = false;
	bool IsSettings = false;
	bool IsNewGame = false;
	while (1) {
		//Main Menu
		if (IsMainMenu) {
			H3Handle MainMenuScene = H3_Scene_Create(screen, true);
			H3Handle MainMenu = H3_Object_Create2(MainMenuScene, "MainMenu", NULL, 1);
			H3_Object_AddComponent(MainMenu, MAINMENUCOMPONENT_CREATE(&IsMainMenu,&IsCredits,&IsSettings,&IsNewGame));
			H3Handle InstanceCode = H3_Object_Create2(MainMenuScene, "InstanceCode", NULL, 1);
			H3_Object_AddComponent(InstanceCode, TEXTCOMPONENT_CREATE("2022-I1, by T-REX 3008", textprops));
			H3_Object_SetTranslation(InstanceCode, 220, 900);
			H3Handle GameName = H3_Object_Create2(MainMenuScene, "GameName", NULL, 1);
			H3_Object_AddComponent(GameName, TEXTCOMPONENT_CREATE("GAMENAME PLACEHOLDER", textprops));
			H3_Object_SetTranslation(GameName, 950, 100);
			H3Handle LaHordeLogo = H3_Object_Create2(MainMenuScene, "LaHordeLogo", NULL, 1);
			H3_Object_AddComponent(LaHordeLogo, SPRITECOMPONENT_CREATE("assets/LaHordeLogo.png",0x22));
			H3_Object_SetTranslation(LaHordeLogo, 1800, 900);
			while (IsMainMenu) {
				H3_DoFrame(screen, MainMenuScene);
			}
			H3_Scene_Destroy(MainMenuScene);
		}

		//credits
		if (IsCredits) {
			H3Handle CreditsScene = H3_Scene_Create(screen, true);
			H3Handle Credits = H3_Object_Create2(CreditsScene, "Credits", NULL, 1);
			H3_Object_AddComponent(Credits, CREDITSCOMPONENT_CREATE(&IsCredits,&IsMainMenu));
			H3Handle CreditText = H3_Object_Create2(CreditsScene, "CreditsText", NULL, 1);
			H3_Object_AddComponent(CreditText, TEXTCOMPONENT_CREATE("A big thanks to Nicolas, Arthur, Dorian and Ruben\n who worked really hard.\n\n As well as to limezu on itch.io whose\n assetpack we couldn't have done without", textprops));
			H3_Object_SetTranslation(CreditText, 950, 300);
			while (IsCredits) {
				H3_DoFrame(screen, CreditsScene);
			}
			H3_Scene_Destroy(CreditsScene);
		}

		//settings
		if (IsSettings) {
			H3Handle SettingsScene = H3_Scene_Create(screen, true);
			H3Handle Settings = H3_Object_Create2(SettingsScene, "Settings", NULL, 1);
			H3_Object_AddComponent(Settings, SETTINGSCOMPONENT_CREATE(&IsSettings, &IsMainMenu));
			while (IsSettings) {
				H3_DoFrame(screen, SettingsScene);
			}
			H3_Scene_Destroy(SettingsScene);
		}

		//game
		if (IsNewGame) {
			H3Handle GameScene = H3_Scene_Create(screen, true);
			while (IsNewGame) {
				H3_DoFrame(screen, GameScene);
			}
			H3_Scene_Destroy(GameScene);
		}
	}
}
