#include <h3.h>
#include <math.h>
#include "components/MainMenu.h"
#include "components/Credits.h"
#include "components/Settings.h"
#include "components/textcomponent.h"
#include "components/spritecomponent.h"
#include "components/maplayercomponent.h"
#include "components/energycomponent.h"
#include "components/mycameracomponent.h"
#include "components/ClockComponent.h"
#include "components/playercomponent.h"
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
			uint32_t barWidth, barHeight;
			uint32_t backBarWidth, backBarHeight;
			H3Handle fullBar = H3_Texture_Load("assets/AllBar.png", &barWidth, &barHeight);
			H3Handle backBar = H3_Texture_Load("assets/EmptyBar.png", &backBarWidth, &backBarHeight);

			

			//temp Map init
			H3Handle map = H3_Map_Load("assets/map.tmx");
			H3_Map_RegisterObjectLayerForPhysicsInScene(GameScene, map, "collider");
			H3Handle mapplayer = H3_Object_Create2(GameScene, "layer floor", NULL,1);
			H3_Object_AddComponent(mapplayer, MAPLAYERCOMPONENT_CREATE(map, "floor"));
			H3Handle mapplayer1 = H3_Object_Create2(GameScene, "layer object", NULL,4);
			H3_Object_AddComponent(mapplayer1, MAPLAYERCOMPONENT_CREATE(map, "object"));
			H3Handle mapplayer2 = H3_Object_Create2(GameScene, "layer object up", NULL,4);
			H3_Object_AddComponent(mapplayer2, MAPLAYERCOMPONENT_CREATE(map, "object up"));
			H3Handle mapplayer3 = H3_Object_Create2(GameScene, "layer wall", NULL,4);
			H3_Object_AddComponent(mapplayer3, MAPLAYERCOMPONENT_CREATE(map, "wall"));

			//player
			H3Handle player = H3_Object_Create2(GameScene, "player", NULL,3);
			H3_Object_AddComponent(player, SPRITECOMPONENT_CREATE("assets/p.png", 0x22));
			H3_Object_EnablePhysics(player, H3_BOX_COLLIDER(CDT_Dynamic, 25, 35, 0x22, false));
			H3_Object_AddComponent(player, PLAYERCOMPONENT_CREATE());
			H3_Object_SetTranslation(player, 960, 540);

			//bar of tiredness
			H3Handle energyBar = H3_Object_Create2(GameScene, "energybar", NULL, 5);
			H3_Object_AddComponent(energyBar, ENERGYCOMPONENT_CREATE(fullBar, backBar));
			H3_Object_SetTranslation(energyBar, 30, 5);

			//camera 
			H3Handle camera = H3_Object_Create(GameScene, "camera", NULL);
			H3_Object_AddComponent(camera, MYCAMERACOMPONENT_CREATE(960, 540, player));

			//Time
			H3Handle time = H3_Object_Create2(GameScene, "Clock", camera, 5);
			H3_Object_AddComponent(time, CLOCKCOMPONENT_CREATE(&textprops));
			while (IsNewGame) {
				H3_DoFrame(screen, GameScene);
			}
			H3_Texture_Destroy(fullBar);
			H3_Texture_Destroy(backBar);
			H3_Scene_Destroy(GameScene);
		}
	}
}
