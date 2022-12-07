#include <stdio.h>
#include <stdlib.h>
#include <h3.h>
#include <components/MAPLAYERCOMPONENT.h>
#include <components/spritecomponent.h>
#include <math.h>
#include <components/cameracomponent.h>

#ifndef NDEBUG
# pragma comment(lib, "h3-s-d.lib")
#else // !NDEBUG
# pragma comment(lib, "h3-s.lib")
#endif // !NDEBUG 

int main(int argc, char** argv)
{
	//variable
	float screen_width = 1920;
	float screen_height = 1080;
	int mx;
	int my;
	float px = screen_width / 2;
	float py = screen_height / 2;

	//fenêtre
	bool jeu = true;
	H3Handle screen = H3_Init((SH3InitParams)
	{
		.width = screen_width,
			.height = screen_height,
			.windowTitle = "pandemie",
			.fullscreen = false

	});

	H3Handle scene = H3_Scene_Create(screen, true);

	//map
	H3Handle map = H3_Map_Load("assets/map.tmx");
	H3_Map_RegisterObjectLayerForPhysicsInScene(scene, map, "collider");
	H3Handle mapplayer = H3_Object_Create(scene, "layer floor", NULL);
	H3_Object_AddComponent(mapplayer, MAPLAYERCOMPONENT_CREATE(map, "floor"));
	H3Handle mapplayer1 = H3_Object_Create(scene, "layer object", NULL);
	H3_Object_AddComponent(mapplayer1, MAPLAYERCOMPONENT_CREATE(map, "object"));
	H3Handle mapplayer2 = H3_Object_Create(scene, "layer object up", NULL);
	H3_Object_AddComponent(mapplayer2, MAPLAYERCOMPONENT_CREATE(map, "object up"));
	H3Handle mapplayer3 = H3_Object_Create(scene, "layer wall", NULL);
	H3_Object_AddComponent(mapplayer3, MAPLAYERCOMPONENT_CREATE(map, "wall"));


	//player
	H3Handle player = H3_Object_Create(scene, "player", NULL);
	H3_Object_AddComponent(player, SPRITECOMPONENT_CREATE("assets/p.png", 0x22));
	H3_Object_EnablePhysics(player, H3_BOX_COLLIDER(CDT_Dynamic, 25, 35, 0x22, false));
	H3_Object_SetTranslation(player, px, py);

	//camera 
	H3Handle camera = H3_Object_Create(scene, "camera", NULL);
	H3_Object_AddComponent(camera, CAMERACOMPONENT_CREATE(screen_width / 2, screen_height / 2));
	H3_Object_SetTranslation(camera, px, py);

	




	//boucle infini et mouvement
	while (jeu)
	{
		
		jeu = H3_DoFrame(screen, scene);
	}


}

