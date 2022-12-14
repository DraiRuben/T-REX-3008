#include <components/playercomponent.h>

#include <components/tirednesscomponent.h>
#include <components/animatedspritecomponent.h>

#include <stdlib.h>

typedef struct
{
	int player;
	float speed;
	float pvx, pvy;
	bool IsSprint;
	bool IsShift;
	bool IsMovingV;
	bool IsMovingH;
	int direction; //0 = right, 1 = up, 2 = left, 3 = down
	int RunSFXonce;
	int WalkSFXonce;
	float slowdown; // slowdown when tiredness is high

	//anim texture
	uint32_t TxW, TxH;
	H3Handle TxRunDown;
	H3Handle TxRunUp;
	H3Handle TxRunLeft;
	H3Handle TxRunRight;
	H3Handle TxIdleDown;
	H3Handle TxIdleUp;
	H3Handle TxIdleLeft;
	H3Handle TxIdleRight;

	bool* pt_isWin;
	bool* pt_isEnd;
	bool* pt_isGame;
	H3Handle energyBar;
	H3Handle RunSFX;
	H3Handle WalkSFX;
} PlayerComponent_Properties;

void PlayerComponent_Terminate(void* properties)
{
	free(properties);
}

void PlayerComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	PlayerComponent_Properties* props = (PlayerComponent_Properties*)properties;

	//movement horizontal
	if (H3_Input_IsKeyDown(K_Q) || H3_Input_IsKeyDown(K_A) || H3_Input_IsGamepadBtnDown(GB_DPad_Left) || H3_Input_IsKeyDown(K_Left))
	{
		props->IsMovingH = true;
		props->pvx = -125;
		props->direction = 2; // left
	}
	else if (H3_Input_IsKeyDown(K_D) || H3_Input_IsKeyDown(K_Right) || H3_Input_IsGamepadBtnDown(GB_DPad_Right))
	{
		props->IsMovingH = true;
		props->pvx = 125;
		props->direction = 0; // right
	}
	else
	{
		props->IsMovingH = false;
		props->pvx = 0;
	}

	//movement Vertical
	if (H3_Input_IsKeyDown(K_Z)|| H3_Input_IsKeyDown(K_W)||H3_Input_IsGamepadBtnDown(GB_DPad_Up)|| H3_Input_IsKeyDown(K_Up))
	{
		props->IsMovingV = true;
		props->pvy = -125;
		props->direction = 1; // up
	}
	else if (H3_Input_IsKeyDown(K_S)||H3_Input_IsKeyDown(K_Down) || H3_Input_IsGamepadBtnDown(GB_DPad_Down))
	{
		props->IsMovingV = true;
		props->pvy = 125;
		props->direction = 3; // down
	}
	else
	{
		props->IsMovingV = false;
		props->pvy = 0;
	}

	//sprint
	if (H3_Input_IsKeyDown(K_Shift) || H3_Input_IsGamepadBtnDown(GB_A))
	{
		props->speed = 1.65 * props->slowdown;
		props->IsShift = true;
		
	}
	else
	{
		props->speed = 1 * props->slowdown;
		props->IsShift = false;
	}
	if ((props->IsMovingH || props->IsMovingV) && props->IsShift) {
		H3_Sound_Stop(props->WalkSFX);
		props->WalkSFXonce = 0;
		props->IsSprint = true;
		props->RunSFXonce += 1;
	}
	else if (props->IsMovingH || props->IsMovingV) {
		H3_Sound_Stop(props->RunSFX);
		props->RunSFXonce = 0;
		props->WalkSFXonce += 1;
		props->IsSprint = false;
	}
	else {
		props->WalkSFXonce = 0;
		props->RunSFXonce = 0;
		H3_Sound_Stop(props->RunSFX);
		H3_Sound_Stop(props->WalkSFX);
		props->IsSprint = false;
	}
	if (props->RunSFXonce==1) {
		H3_Sound_Play(props->RunSFX, 80, true);
	}
	if (props->WalkSFXonce == 1) {
		H3_Sound_Play(props->WalkSFX, 80, true);
	}
	H3_Object_SetVelocity(object, props->pvx * props->speed, props->pvy * props->speed);

	//player death
	if (TirednessComponent_GettirednessEx(props->energyBar) >= 1)
	{
		H3_Sound_Stop(props->WalkSFX);
		H3_Sound_Stop(props->RunSFX);
		*props->pt_isWin	= false;
		*props->pt_isGame	= false;
		*props->pt_isEnd	= true;
	}

	//animation
	if (props->IsMovingH || props->IsMovingV) {
		//run
		if (props->direction == 1) {
			AnimatedSpriteComponent_SetTextureEx(object, props->TxRunUp);
		}
		else if (props->direction == 3) {
			AnimatedSpriteComponent_SetTextureEx(object, props->TxRunDown);
		}
		else if (props->direction == 2) {
			AnimatedSpriteComponent_SetTextureEx(object, props->TxRunLeft);
		}
		else if (props->direction == 0) {
			AnimatedSpriteComponent_SetTextureEx(object, props->TxRunRight);
		}
	}
	else {
		//idle
		if (props->direction == 1) {
			AnimatedSpriteComponent_SetTextureEx(object, props->TxIdleUp);
		}
		else if (props->direction == 3) {
			AnimatedSpriteComponent_SetTextureEx(object, props->TxIdleDown);
		}
		else if (props->direction == 2) {
			AnimatedSpriteComponent_SetTextureEx(object, props->TxIdleLeft);
		}
		else if (props->direction == 0) {
			AnimatedSpriteComponent_SetTextureEx(object, props->TxIdleRight);
		}
	}

	//accelerate anim when sprint
	if (props->IsSprint)
		AnimatedSpriteComponent_SetFrameDurationEx(object, 0.1);
	else
		AnimatedSpriteComponent_SetFrameDurationEx(object, 0.2);
}

void* PlayerComponent_CreateProperties(bool* isWin, bool* isEndGame, bool* isInGame, H3Handle energyBarRef)
{
	PlayerComponent_Properties* properties = malloc(sizeof(PlayerComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->RunSFX = H3_Sound_Load("assets/SFX/RunSFX.wav");
	properties->WalkSFX = H3_Sound_Load("assets/SFX/WalkSFX.wav");
	properties->RunSFXonce = 0;
	properties->WalkSFXonce = 0;
	properties->pt_isWin	= isWin;
	properties->pt_isEnd	= isEndGame;
	properties->pt_isGame	= isInGame;
	properties->energyBar	= energyBarRef;
	properties->speed = 1;
	properties->IsMovingV = false;
	properties->IsMovingH = false;
	properties->IsSprint = false;
	properties->IsShift = false;

	//load texture anim
	properties->TxRunDown	= H3_Texture_Load("assets/Sprites/player/PlayerMovefront.png", &properties->TxW, &properties->TxH);
	properties->TxRunUp		= H3_Texture_Load("assets/Sprites/player/PlayerMoveBehind.png", &properties->TxW, &properties->TxH);
	properties->TxRunLeft	= H3_Texture_Load("assets/Sprites/player/PlayerMoveLeft.png", &properties->TxW, &properties->TxH);
	properties->TxRunRight	= H3_Texture_Load("assets/Sprites/player/PlayerMoveRight.png", &properties->TxW, &properties->TxH);
	properties->TxIdleDown	= H3_Texture_Load("assets/Sprites/player/PlayerIdleFront.png", &properties->TxW, &properties->TxH);
	properties->TxIdleUp	= H3_Texture_Load("assets/Sprites/player/PlayerIdlebehind.png", &properties->TxW, &properties->TxH);
	properties->TxIdleLeft	= H3_Texture_Load("assets/Sprites/player/PlayerIdleLeft.png", &properties->TxW, &properties->TxH);
	properties->TxIdleRight	= H3_Texture_Load("assets/Sprites/player/playerIdleRight.png", &properties->TxW, &properties->TxH);


	properties->direction = 3; //down
	return properties;
}
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(PlayerComponent,PLAYERCOMPONENT_TYPEID, bool, IsSprint);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(PlayerComponent, PLAYERCOMPONENT_TYPEID, float, slowdown);