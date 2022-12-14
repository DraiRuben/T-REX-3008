#include <h3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "components/MainMenu.h"
#include "components/Credits.h"
#include "components/Settings.h"
#include "components/tirednesscomponent.h"
#include "components/mycameracomponent.h"
#include "components/ClockComponent.h"
#include "components/playercomponent.h"
#include "components/EnemyComponent.h"
#include "components/inventorycomponent.h"
#include "components/collectablecomponent.h"
#include "components/SpawnerComponent.h"
#include "components/monsterecomponent.h"
#include "components/aislespawnercomponent.h"
#include "components/cashregistercomponent.h"
#include "components/digicodecomponent.h"

#include "components/textcomponent.h"
#include "components/spritecomponent.h"
#include "components/maplayercomponent.h"
#include "components/cameracomponent.h"
#include "components/animatedspritecomponent.h"

#ifndef NDEBUG
# pragma comment(lib, "h3-s-d.lib")
#else // !NDEBUG
# pragma comment(lib, "h3-s.lib")
#endif // !NDEBUG

int main()
{
	//randow init
	srand(time(NULL));

	//Text props Init
	SH3TextProperties textprops = (SH3TextProperties){
			.font = H3_Font_Load("assets/Fonts/Toriko.ttf"),
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
			.font = H3_Font_Load("assets/Fonts/Comfortaa-regular.ttf"),
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
	bool IsEndGame = false;
	bool IsWin = false;

	char FinalTime[256];
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
			H3_Object_AddComponent(LaHordeLogo, SPRITECOMPONENT_CREATE("assets/Menu/LaHordeLogo.png",0x22));
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
			H3Handle fullBar = H3_Texture_Load("assets/Sprites/AllBar.png", &barWidth, &barHeight);
			H3Handle backBar = H3_Texture_Load("assets/Sprites/EmptyBar.png", &backBarWidth, &backBarHeight);

			
			//music
			H3Handle music = H3_Music_Load("assets/SFX/AmbiantSFX.wav");
			H3_Music_Play(music, 1, true);
			//Map init
			H3Handle map = H3_Map_Load("assets/Map/map.tmx");
			H3_Map_RegisterObjectLayerForPhysicsInScene(GameScene, map, "collider");
			H3Handle maplayer4 = H3_Object_Create2(GameScene, "layer carpet", NULL, 2);
			H3_Object_AddComponent(maplayer4, MAPLAYERCOMPONENT_CREATE(map, "carpet"));
			H3Handle maplayer = H3_Object_Create2(GameScene, "layer floor", NULL, 1);
			H3_Object_AddComponent(maplayer, MAPLAYERCOMPONENT_CREATE(map, "floor"));
			H3Handle maplayer3 = H3_Object_Create2(GameScene, "layer wall", NULL, 5);
			H3_Object_AddComponent(maplayer3, MAPLAYERCOMPONENT_CREATE(map, "wall"));
			H3Handle maplayer1 = H3_Object_Create2(GameScene, "layer object", NULL, 5);
			H3_Object_AddComponent(maplayer1, MAPLAYERCOMPONENT_CREATE(map, "object"));
			H3Handle maplayer2 = H3_Object_Create2(GameScene, "layer object up", NULL, 6);
			H3_Object_AddComponent(maplayer2, MAPLAYERCOMPONENT_CREATE(map, "object up"));
			
			//Random Aisle Init
			H3Handle AisleSpawner = H3_Object_Create(GameScene, "AisleSpawner", NULL);
			H3_Object_AddComponent(AisleSpawner, AISLESPAWNERCOMPONENT_CREATE(&GameScene));

			//related objects
			H3Handle player = H3_Object_Create2(GameScene, "player", NULL,3);
			H3Handle camera = H3_Object_Create(GameScene, "camera", NULL);
			H3Handle emptyBar = H3_Object_Create2(GameScene, "emptyBar", camera, 10);
			H3Handle energyBar = H3_Object_Create2(GameScene, "energybar", camera, 10);

			//player
			H3_Object_AddComponent(player, ANIMATEDSPRITECOMPONENT_CREATE("assets/Sprites/player/PlayerMovefront.png", 0x22, 6, 0.2, true));
			H3_Object_EnablePhysics(player, H3_BOX_COLLIDER(CDT_Dynamic, 25, 38, 0x22, false));
			H3_Object_AddComponent(player, PLAYERCOMPONENT_CREATE(&IsWin, &IsEndGame, &IsNewGame, energyBar));
			H3_Object_AddComponent(player, INVENTORYCOMPONENT_CREATE(&GameScene,&energyBar));
			H3_Object_SetTranslation(player, 1750, 2100);




			//crowbar temporaire
			H3Handle crowbar = H3_Object_Create2(GameScene, "crowbar", NULL, 3);
			H3_Object_AddComponent(crowbar, SPRITECOMPONENT_CREATE("assets/Objects/PiedDeBiche.png", 0x22));
			H3_Object_AddComponent(crowbar, COLLECTABLECOMPONENT_CREATE(14, 1, NULL));
			H3_Object_EnablePhysics(crowbar, H3_BOX_COLLIDER(CDT_Dynamic, 16, 16, 0x22, true));
			H3_Object_SetTranslation(crowbar, 39*32, 5*32);




			//camera
			H3_Object_AddComponent(camera, MYCAMERACOMPONENT_CREATE(480, 270, player));
			H3_Object_SetTranslation(camera, 1850, 1125);

			//bar of tiredness
			H3_Object_AddComponent(emptyBar, SPRITECOMPONENT_CREATE("assets/Sprites/EmptyBar.png", A_Left + A_Top));
			H3_Object_SetTranslation(emptyBar, -235, -130);
			H3_Object_AddComponent(energyBar, TIREDNESSCOMPONENT_CREATE(fullBar,player,camera,GameScene));
			H3_Object_SetTranslation(energyBar, -234, -128);

			//enemies init
			bool IsNewWave = false;
			bool IsWave = false;
			bool GlobalAggro = false;
			H3Handle spawner = H3_Object_Create2(GameScene, "Spawner", NULL, 3);
			H3_Object_AddComponent(spawner, SPAWNERCOMPONENT_CREATE(&player, &GameScene,energyBar,&IsNewWave,&IsWave,&GlobalAggro));
			
			//Time
			H3Handle time = H3_Object_Create2(GameScene, "Clock", camera, 10);
			H3_Object_AddComponent(time, CLOCKCOMPONENT_CREATE(&clockprops, &IsNewWave));

			//Inventory Slots
			H3Handle slot0 = H3_Object_Create2(GameScene, "inventory, slot 0", camera, 10);	//pocket 2
			H3Handle slot1 = H3_Object_Create2(GameScene, "inventory, slot 1", camera, 10);	//pocket 1
			H3Handle slot2 = H3_Object_Create2(GameScene, "inventory, slot 2", camera, 10); //hand
			H3_Object_AddComponent(slot0, SPRITECOMPONENT_CREATE("assets/Sprites/UI_inventorySlot.png", 0x22));
			H3_Object_AddComponent(slot1, SPRITECOMPONENT_CREATE("assets/Sprites/UI_inventorySlot.png", 0x22));
			H3_Object_AddComponent(slot2, SPRITECOMPONENT_CREATE("assets/Sprites/UI_MainSlot_Inventory.png", 0x22));
			H3_Object_SetTranslation(slot0, 220, -115);
			H3_Object_SetTranslation(slot1, 185, -115);
			H3_Object_SetTranslation(slot2, 150, -115);
			
			//shadow effect
			H3Handle gradient = H3_Object_Create2(GameScene, "gradient", camera, 9);
			H3Handle gradient1 = H3_Object_Create2(GameScene, "gradient1", camera, 9);
			H3Handle gradient2 = H3_Object_Create2(GameScene, "gradient2", camera, 9);
			H3Handle gradient3 = H3_Object_Create2(GameScene, "gradient3", camera, 9);
			H3Handle gradient4 = H3_Object_Create2(GameScene, "gradient4", camera, 9);
			H3Handle gradient5 = H3_Object_Create2(GameScene, "gradient5", camera, 9);

			H3_Object_AddComponent(gradient, SPRITECOMPONENT_CREATE("assets/Sprites/gradien.png", 0x22));
			H3_Object_AddComponent(gradient1, SPRITECOMPONENT_CREATE("assets/Sprites/gradien.png", 0x22));
			H3_Object_AddComponent(gradient2, SPRITECOMPONENT_CREATE("assets/Sprites/gradien.png", 0x22));
			H3_Object_AddComponent(gradient3, SPRITECOMPONENT_CREATE("assets/Sprites/gradien.png", 0x22));
			H3_Object_AddComponent(gradient4, SPRITECOMPONENT_CREATE("assets/Sprites/gradien.png", 0x22));
			H3_Object_AddComponent(gradient5, SPRITECOMPONENT_CREATE("assets/Sprites/gradien.png", 0x22));

			//Digicodes
			H3Handle digicode = H3_Object_Create2(GameScene, "digicode", camera, 10);
			H3_Object_AddComponent(digicode, SPRITECOMPONENT_CREATE("assets/Sprites/Digicode.png", 0x22));
			H3_Object_AddComponent(digicode, DIGICODECOMPONENT_CREATE(camera));
			H3_Object_SetTranslation(digicode, -185, 0);
			H3_Object_SetEnabled(digicode, false);

			//cash registers
			bool cashregisterIsOpen = false;
			H3Handle cashregister0 = H3_Object_Create2(GameScene, "caisse0", NULL, 4);
			H3Handle cashregister1 = H3_Object_Create2(GameScene, "caisse1", NULL, 4);
			H3Handle cashregister2 = H3_Object_Create2(GameScene, "caisse2", NULL, 4);
			H3Handle cashregister3 = H3_Object_Create2(GameScene, "caisse3", NULL, 4);
			H3Handle cashregister4 = H3_Object_Create2(GameScene, "caisse4", NULL, 4);
			H3Handle cashregister5 = H3_Object_Create2(GameScene, "caisse5", NULL, 4);
			H3_Object_AddComponent(cashregister0, CASHREGISTERCOMPONENT_CREATE(digicode, &cashregisterIsOpen,&GameScene,&player));
			H3_Object_AddComponent(cashregister1, CASHREGISTERCOMPONENT_CREATE(digicode, &cashregisterIsOpen, &GameScene, &player));
			H3_Object_AddComponent(cashregister2, CASHREGISTERCOMPONENT_CREATE(digicode, &cashregisterIsOpen, &GameScene, &player));
			H3_Object_AddComponent(cashregister3, CASHREGISTERCOMPONENT_CREATE(digicode, &cashregisterIsOpen, &GameScene, &player));
			H3_Object_AddComponent(cashregister4, CASHREGISTERCOMPONENT_CREATE(digicode, &cashregisterIsOpen, &GameScene, &player));
			H3_Object_AddComponent(cashregister5, CASHREGISTERCOMPONENT_CREATE(digicode, &cashregisterIsOpen, &GameScene, &player));
			H3_Object_EnablePhysics(cashregister0, H3_BOX_COLLIDER(CDT_Static, 32, 32, A_Top + A_Left, true));
			H3_Object_EnablePhysics(cashregister1, H3_BOX_COLLIDER(CDT_Static, 32, 32, A_Top + A_Left, true));
			H3_Object_EnablePhysics(cashregister2, H3_BOX_COLLIDER(CDT_Static, 32, 32, A_Top + A_Left, true));
			H3_Object_EnablePhysics(cashregister3, H3_BOX_COLLIDER(CDT_Static, 32, 32, A_Top + A_Left, true));
			H3_Object_EnablePhysics(cashregister4, H3_BOX_COLLIDER(CDT_Static, 32, 32, A_Top + A_Left, true));
			H3_Object_EnablePhysics(cashregister5, H3_BOX_COLLIDER(CDT_Static, 32, 32, A_Top + A_Left, true));
			H3_Object_SetTranslation(cashregister0, 1504, 1248);
			H3_Object_SetTranslation(cashregister1, 1504, 1376);
			H3_Object_SetTranslation(cashregister2, 1504, 1536);
			H3_Object_SetTranslation(cashregister3, 1504, 1696);
			H3_Object_SetTranslation(cashregister4, 1504, 1856);
			H3_Object_SetTranslation(cashregister5, 1504, 2016);

			//game loop
			while (IsNewGame) {
				H3_DoFrame(screen, GameScene);
				sprintf_s(FinalTime, 256, TextComponent_GetTextEx(time));
			}
			H3_Music_Stop(music);
			H3_Scene_Destroy(GameScene);
		}

		//end Game Menu
		if (IsEndGame) {
			H3Handle EndGameScene = H3_Scene_Create(screen, true);
			H3Handle cam = H3_Object_Create(EndGameScene, "cam", NULL);
			H3_Object_AddComponent(cam, CAMERACOMPONENT_CREATE(480,270));
			H3Handle EndTime = H3_Object_Create(EndGameScene, "EndTime", cam);
			H3_Object_AddComponent(EndTime, TEXTCOMPONENT_CREATE(FinalTime, clockprops));
			H3_Object_SetTranslation(EndTime, 0, -110);
			while (IsEndGame)
			{
				H3_DoFrame(screen, EndGameScene);
			}
			H3_Scene_Destroy(EndGameScene);
		}
	}
}
