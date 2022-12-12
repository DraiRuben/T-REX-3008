#include <components/playercomponent.h>

#include <components/tirednesscomponent.h>

#include <stdlib.h>

typedef struct
{
	int player;
	float speed;
	float pvx, pvy;
	bool IsSprint;
	float slowdown; // slowdown when tiredness is high

	bool* pt_isWin;
	bool* pt_isEnd;
	bool* pt_isGame;
	H3Handle energyBar;
} PlayerComponent_Properties;

void PlayerComponent_Terminate(void* properties)
{
	free(properties);
}

void PlayerComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	PlayerComponent_Properties* props = (PlayerComponent_Properties*)properties;

	//movement Vertical
	if (H3_Input_IsKeyDown(K_Z)|| H3_Input_IsKeyDown(K_W)||H3_Input_IsGamepadBtnDown(GB_DPad_Up)|| H3_Input_IsKeyDown(K_Up))
	{
		props->pvy = -125;
	}
	else if (H3_Input_IsKeyDown(K_S)||H3_Input_IsKeyDown(K_Down) || H3_Input_IsGamepadBtnDown(GB_DPad_Down))
	{
		props->pvy = 125;
	}
	else
	{
		props->pvy = 0;
	}

	//movement horizontal
	if (H3_Input_IsKeyDown(K_Q)||H3_Input_IsKeyDown(K_A) || H3_Input_IsGamepadBtnDown(GB_DPad_Left) || H3_Input_IsKeyDown(K_Left))
	{
		props->pvx = -125;
	}
	else if (H3_Input_IsKeyDown(K_D) || H3_Input_IsKeyDown(K_Right) || H3_Input_IsGamepadBtnDown(GB_DPad_Right))
	{
		props->pvx = 125;
	}
	else
	{
		props->pvx = 0;
	}
	//sprint
	if (H3_Input_IsKeyDown(K_Shift) || H3_Input_IsGamepadBtnDown(GB_A))
	{
		props->speed = 1.65 * props->slowdown;
		props->IsSprint = true;
	}
	else
	{
		props->speed = 1 * props->slowdown;
		props->IsSprint = false;
	}

	H3_Object_SetVelocity(object, props->pvx * props->speed, props->pvy * props->speed);

	//player death
	if (TirednessComponent_GettirednessEx(props->energyBar) >= 1)
	{
		*props->pt_isWin	= false;
		*props->pt_isGame	= false;
		*props->pt_isEnd	= true;
	}
}

void* PlayerComponent_CreateProperties(bool* isWin, bool* isEndGame, bool* isInGame, H3Handle energyBarRef)
{
	PlayerComponent_Properties* properties = malloc(sizeof(PlayerComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->pt_isWin	= isWin;
	properties->pt_isEnd	= isEndGame;
	properties->pt_isGame	= isInGame;
	properties->energyBar	= energyBarRef;

	properties->speed = 1;
	properties->IsSprint = false;

	return properties;
}
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(PlayerComponent,PLAYERCOMPONENT_TYPEID, bool, IsSprint);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(PlayerComponent, PLAYERCOMPONENT_TYPEID, float, slowdown);