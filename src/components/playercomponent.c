#include <components/playercomponent.h>

#include <stdlib.h>

typedef struct
{
	int player;
	float pvx, pvy;
	float speed;
	
} PlayerComponent_Properties;

void PlayerComponent_Terminate(void* properties)
{
	free(properties);
}

void PlayerComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	PlayerComponent_Properties* props = (PlayerComponent_Properties*)properties;

	//movement Vertical
	if (H3_Input_IsKeyDown(K_Z)) 
	{
		props->pvy = -125;
	}
	else if (H3_Input_IsKeyDown(K_S))
	{
		props->pvy = 125;
	}
	else
	{
		props->pvy = 0;
	}

	//movement horizontal
	if (H3_Input_IsKeyDown(K_Q))
	{
		props->pvx = -125;
	}
	else if (H3_Input_IsKeyDown(K_D))
	{
		props->pvx = 125;
	}
	else
	{
		props->pvx = 0;
	}

	//sprint
	if (H3_Input_IsKeyDown(K_Shift))
	{
		props->speed = 1.65;
	}
	else
	{
		props->speed = 1;
	}

	H3_Object_SetVelocity(object, props->pvx * props->speed, props->pvy * props->speed);


		

}

void* PlayerComponent_CreateProperties()
{
	PlayerComponent_Properties* properties = malloc(sizeof(PlayerComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->speed = 1;
	return properties;
}

//H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW(PlayerComponent, float, Width);
//H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW(PlayerComponent, float, Height);
//H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(PlayerComponent, PLAYERCOMPONENT_TYPEID, float, Width);
//H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(PlayerComponent, PLAYERCOMPONENT_TYPEID, float, Height);
