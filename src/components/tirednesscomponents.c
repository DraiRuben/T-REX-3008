#include <components/tirednesscomponent.h>

#include <components/mancomponent.h>
#include <components/spritecomponent.h>

#include <stdlib.h>

typedef struct
{
	H3Handle	tirednessBar;
	H3Handle	player;
	H3Handle	camera;
	H3Handle	scene;

	float		tiredness;	// 100% tiredness = 1.0f
	float		counter;
	int			recovEnergy;
	H3Handle	iconEnergy1;
	H3Handle	iconEnergy2;
	bool		doOnce;
} TirednessComponent_Properties;

void TirednessComponent_Terminate(void* properties)
{
	TirednessComponent_Properties* props = (TirednessComponent_Properties*)properties;
	
	if (props->tirednessBar)
		H3_Texture_Destroy(props->tirednessBar);
	
	free(properties);
}

void TirednessComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties)
{
	TirednessComponent_Properties* props = (TirednessComponent_Properties*)properties;

	H3_Texture_DrawRectEx(h3, transform, props->tirednessBar, 0.0f, 0.0f, props->tiredness, 1.0f, A_Left + A_Top);
}

void TirednessComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	TirednessComponent_Properties* props = (TirednessComponent_Properties*)properties;

	//create energy icons one time
	if (!props->doOnce)
	{
		props->doOnce = true;

		props->iconEnergy1 = H3_Object_Create2(props->scene, "first energy", props->camera, 15);
		H3_Object_AddComponent(props->iconEnergy1, SPRITECOMPONENT_CREATE("assets/Sprites/UI_Energy.png", 0x22));
		H3_Object_SetTranslation(props->iconEnergy1, -232, -118);
		H3_Object_Scale(props->iconEnergy1, 0.25f);
		H3_Object_SetEnabled(props->iconEnergy1, false);

		props->iconEnergy2 = H3_Object_Create2(props->scene, "second energy", props->camera, 15);
		H3_Object_AddComponent(props->iconEnergy2, SPRITECOMPONENT_CREATE("assets/Sprites/UI_Energy.png", 0x22));
		H3_Object_SetTranslation(props->iconEnergy2, -224, -118);
		H3_Object_Scale(props->iconEnergy2, 0.25f);
		H3_Object_SetEnabled(props->iconEnergy2, false);
	}

	//filled tiredness bar
	if (props->tiredness < 1)
	{
		if (ManComponent_GetIsSprintEx(props->player))
			props->tiredness += 0.01f * H3_GetDeltaTime(); //fill 1%/s	 in sprint
		else
			props->tiredness += 0.001f * H3_GetDeltaTime();//fill 0.1%/s in normal run
	}

	//reduce tiredness when player drink 1 monstere
	if (props->recovEnergy >= 1)
	{
		//time limit to reduce tiredness
		props->counter += H3_GetDeltaTime();
		if (props->counter >= 10)
		{
			props->recovEnergy--;
			props->counter = 0.0f;
		}

		//show an icon
		H3_Object_SetEnabled(props->iconEnergy1, true);

		//reduce tiredness
		if (props->tiredness > 0)
			props->tiredness -= 0.009f * H3_GetDeltaTime(); //remove 0.9%/s with one monstere (cumulative)
	}
	else
		H3_Object_SetEnabled(props->iconEnergy1, false);

	//reduce tiredness when player drink a second monstere
	if (props->recovEnergy >= 2)
	{
		//show an icon
		H3_Object_SetEnabled(props->iconEnergy2, true);

		//reduce tiredness
		if (props->tiredness > 0)
		props->tiredness -= 0.009f * H3_GetDeltaTime(); //remove 0.9%/s with second monstere (cumulative : 1.8%/s)
	}
	else
		H3_Object_SetEnabled(props->iconEnergy2, false);

	//slowdown player when he is tired
	if (props->tiredness > 0.5)
	{
		float slowdown = (1 - props->tiredness) * 1.5 + 0.25;
		ManComponent_SetslowdownEx(props->player, slowdown);
	}
	else
		ManComponent_SetslowdownEx(props->player, 1);
}

void* TirednessComponent_CreateProperties(H3Handle textureBar, H3Handle playerRef, H3Handle cameraRef, H3Handle sceneRef)
{
	TirednessComponent_Properties* properties = malloc(sizeof(TirednessComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->tirednessBar	= textureBar;
	properties->player			= playerRef;
	properties->camera			= cameraRef;
	properties->scene			= sceneRef;

	properties->tiredness	= 0.0f;
	properties->counter		= 0.0f;
	properties->recovEnergy = 0;
	properties->iconEnergy1	= NULL;
	properties->iconEnergy2	= NULL;

	properties->doOnce = false;
	
	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(TirednessComponent, TIREDNESSCOMPONENT_TYPEID, int, recovEnergy);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(TirednessComponent, TIREDNESSCOMPONENT_TYPEID, float, tiredness);