#include <h3.h>

#include <components/tirednesscomponent.h>

#ifndef NDEBUG
# pragma comment(lib, "h3-s-d.lib")
#else // !NDEBUG
# pragma comment(lib, "h3-s.lib")
#endif // !NDEBUG

int main(int argc, char** argv)
{
	//h3
	SH3InitParams window;
	window.fullscreen = false;
	window.height = 1080;
	window.width = 1920;
	window.windowTitle = "Temporaire";

	H3Handle screen = H3_Init(window);

	H3Handle scene = H3_Scene_Create(screen, false);

	uint32_t barWidth,		barHeight;
	uint32_t backBarWidth,	backBarHeight;
	H3Handle fullBar = H3_Texture_Load("assets/AllBar.png", &barWidth, &barHeight);
	H3Handle backBar = H3_Texture_Load("assets/EmptyBar.png", &backBarWidth, &backBarHeight);

	//bar of tiredness
	H3Handle tiredness = H3_Object_Create2(scene, "player", NULL, 3);
	H3_Object_AddComponent(tiredness, TIREDNESSCOMPONENT_CREATE(fullBar, backBar));
	H3_Object_SetTranslation(tiredness, 0, 5);

	while (1)
		H3_DoFrame(screen, scene);
	
	return 0;
}
