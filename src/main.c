#include <h3.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//h3's components
#include "components/textcomponent.h"
#include "components/spritecomponent.h"
#include "components/maplayercomponent.h"

//ruben's components
#include "components/MainMenu.h"
#include "components/Credits.h"
#include "components/Settings.h"
#include "components/ClockComponent.h"
#include "components/EnemyComponent.h"

//arthur's components
#include "components/tirednesscomponent.h"
#include "components/inventorycomponent.h"

//nicolas's components
#include "components/playercomponent.h"
#include "components/mycameracomponent.h"

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
	SH3TextProperties clockprops = (SH3TextProperties){
			.font = H3_Font_Load("assets/Comfortaa-regular.ttf"),
			.size = 12,
			.fillColor = {.r = 255,.g = 255,.b = 255,.a = 255},
			.hasOutline = false,
			.anchor = 0x22,
			.isBold = false,
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
			H3_Object_AddComponent(player, PLAYERCOMPONENT_CREATE());
			H3_Object_AddComponent(player, INVENTORYCOMPONENT_CREATE());
			H3_Object_EnablePhysics(player, H3_BOX_COLLIDER(CDT_Dynamic, 25, 35, 0x22, false));
			H3_Object_SetTranslation(player, 960, 540);

			//camera 
			H3Handle camera = H3_Object_Create(GameScene, "camera", NULL);
			H3_Object_AddComponent(camera, MYCAMERACOMPONENT_CREATE(480, 270, player));

			//bar of tiredness
			H3Handle emptyBar = H3_Object_Create2(GameScene, "emptyBar", camera, 10);
			H3_Object_AddComponent(emptyBar, SPRITECOMPONENT_CREATE("assets/EmptyBar.png", A_Left + A_Top));
			H3_Object_SetTranslation(emptyBar, -235, -130);

			H3Handle tirednessBar = H3_Object_Create2(GameScene, "energybar", camera, 10);
			H3_Object_AddComponent(tirednessBar, TIREDNESSCOMPONENT_CREATE(fullBar, player));
			H3_Object_SetTranslation(tirednessBar, -234, -128);

			//Monstere
			H3Handle monstere = H3_Object_Create2(GameScene, "monstere", NULL, 2);
			H3_Object_AddComponent(monstere, SPRITECOMPONENT_CREATE("assets/monstère.png", 0x22));
			H3_Object_EnablePhysics(monstere, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			H3_Object_Translate(monstere, 100, 100);

			H3Handle monstere1 = H3_Object_Create2(GameScene, "monstere1", NULL, 2);
			H3_Object_AddComponent(monstere1, SPRITECOMPONENT_CREATE("assets/monstère.png", 0x22));
			H3_Object_EnablePhysics(monstere1, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			H3_Object_Translate(monstere1, 150, 100);

			H3Handle monstere2 = H3_Object_Create2(GameScene, "monstere2", NULL, 2);
			H3_Object_AddComponent(monstere2, SPRITECOMPONENT_CREATE("assets/monstère.png", 0x22));
			H3_Object_EnablePhysics(monstere2, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			H3_Object_Translate(monstere2, 100, 150);

			H3Handle monstere3 = H3_Object_Create2(GameScene, "monstere3", NULL, 2);
			H3_Object_AddComponent(monstere3, SPRITECOMPONENT_CREATE("assets/monstère.png", 0x22));
			H3_Object_EnablePhysics(monstere3, H3_BOX_COLLIDER(CDT_Dynamic, 12, 16, 0x22, true));
			H3_Object_Translate(monstere3, 150, 150);

			//Inventory Slots
			H3Handle slot0 = H3_Object_Create2(GameScene, "inventory, slot 0", camera, 10);
			H3Handle slot1 = H3_Object_Create2(GameScene, "inventory, slot 1", camera, 10);
			H3Handle slot2 = H3_Object_Create2(GameScene, "inventory, slot 2", camera, 10);
			H3_Object_AddComponent(slot0, SPRITECOMPONENT_CREATE("assets/UI_inventorySlot.png", 0x22));
			H3_Object_AddComponent(slot1, SPRITECOMPONENT_CREATE("assets/UI_inventorySlot.png", 0x22));
			H3_Object_AddComponent(slot2, SPRITECOMPONENT_CREATE("assets/UI_MainSlot_Inventory.png", 0x22));
			H3_Object_SetTranslation(slot0, 220, -115);
			H3_Object_SetTranslation(slot1, 185, -115);
			H3_Object_SetTranslation(slot2, 150, -115);
			
			//Time
			H3Handle time = H3_Object_Create2(GameScene, "Clock", camera, 5);
			H3_Object_AddComponent(time, CLOCKCOMPONENT_CREATE(&clockprops));
			//raycast init
			int raycast_index = 0;
			//enemies init
			char enemies[256];
			int enemy_index = 0;
			bool IsWave = false;
			for (int i = 0; i < 5; i++) {
				snprintf(enemies, 256, "enemy_%d", enemy_index++);
				H3Handle enemy = H3_Object_Create2(GameScene, enemies, NULL, 3);
				//H3_Object_AddComponent(enemy, ENEMYCOMPONENT_CREATE(&player, &raycast_index, &GameScene));
			}
			while (IsNewGame) {
				H3_DoFrame(screen, GameScene);
			}
			H3_Texture_Destroy(fullBar);
			H3_Texture_Destroy(backBar);
			H3_Scene_Destroy(GameScene);
		}
	}
}
