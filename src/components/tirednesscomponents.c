#include <components/tirednesscomponent.h>

#include <components/playercomponent.h>

#include <stdlib.h>

typedef struct
{
	H3Handle tirednessBar;
	H3Handle player;
	// 100% tiredness = 1.0f
	float tiredness;
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

	if (props->tiredness < 1)
	{
		if (PlayerComponent_GetIsSprintEx(props->player))
			props->tiredness += 0.01f * H3_GetDeltaTime();
		else
			props->tiredness += 0.001f * H3_GetDeltaTime();
	}
}

void* TirednessComponent_CreateProperties(H3Handle textureBar, H3Handle playerRef)
{
	TirednessComponent_Properties* properties = malloc(sizeof(TirednessComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->tirednessBar = textureBar;
	properties->player = playerRef;

	properties->tiredness = 0.0f;

	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(TirednessComponent, TIREDNESSCOMPONENT_TYPEID, float, tiredness);
