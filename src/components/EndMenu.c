#include "components/EndMenu.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct
{
	//required bools to track gamestate
	bool* IsEndMenu;
	bool* IsNewGame;
	bool* IsWin;

	//texture stuff
	uint32_t w, h;
	H3Handle TextureEnd;
	H3Handle NewGameBtn;
	H3Handle ExitBtn;
} EndMenuComponent_Properties;

void EndMenuComponent_Terminate(void* properties)
{
	EndMenuComponent_Properties* props = (EndMenuComponent_Properties*)properties;

	H3_Texture_Destroy(props->TextureEnd);
	H3_Texture_Destroy(props->NewGameBtn);
	H3_Texture_Destroy(props->ExitBtn);
	free(properties);
}

void EndMenuComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties)
{
	//texture EndMenu
	EndMenuComponent_Properties* props = (EndMenuComponent_Properties*)properties;
	H3_Texture_Draw(h3, 0, 0, props->TextureEnd, 0x22);

	//relaunch game
	if (H3_Button(h3, props->NewGameBtn, 800, 400, 0x11)) {
		*props->IsEndMenu = false;
		*props->IsNewGame = true;
		*props->IsWin = false;
	}
	//close game
	if (H3_Button(h3, props->ExitBtn, 800, 700, 0x11)) {
		exit(0);
	}

}

void* EndMenuComponent_CreateProperties(bool* IsEndMenu, bool* IsNewGame, bool* IsWin)
{
	EndMenuComponent_Properties* properties = malloc(sizeof(EndMenuComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	EndMenuComponent_Properties* props = (EndMenuComponent_Properties*)properties;

	//load needed btn images
	properties->TextureEnd = H3_Texture_Load("assets/Menu/bloodscene.png", &props->w, &props->h);
	properties->NewGameBtn = H3_Texture_Load("assets/Menu/NewGame.png", &props->w, &props->h);
	properties->ExitBtn = H3_Texture_Load("assets/Menu/Exit.png", &props->w, &props->h);

	properties->IsEndMenu = IsEndMenu;
	properties->IsNewGame = IsNewGame;
	properties->IsWin = IsWin;
	return properties;
}