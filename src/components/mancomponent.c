#include <components/mancomponent.h>

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

	bool playerWin;
	bool* pt_isWin;
	bool* pt_isEnd;
	bool* pt_isGame;
	H3Handle energyBar;
	H3Handle RunSFX;
	H3Handle WalkSFX;
} ManComponent_Properties;

void ManComponent_Terminate(void* properties)
{
	free(properties);
}

void ManComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	ManComponent_Properties* props = (ManComponent_Properties*)properties;

	//Horizontal Movement
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
		props->IsMovingH = false;//none
		props->pvx = 0;
	}

	//Vertical Movement
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
		props->IsMovingV = false;// none
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
	//enables sprint bool if moving and uses sfx
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

	//sfx play management
	if (props->RunSFXonce==1) {
		H3_Sound_Play(props->RunSFX, 0.2, true);
	}
	if (props->WalkSFXonce == 1) {
		H3_Sound_Play(props->WalkSFX, 0.2, true);
	}

	//apply computed movement to player
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

	//playerWin
	if (props->playerWin)
	{
		H3_Sound_Stop(props->WalkSFX);
		H3_Sound_Stop(props->RunSFX);
		*props->pt_isWin	= true;
		*props->pt_isGame	= false;
		*props->pt_isEnd	= true;
	}

	//animations
	if (props->IsMovingH || props->IsMovingV) {
		//moving
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

void* ManComponent_CreateProperties(bool* isWin, bool* isEndGame, bool* isInGame, H3Handle energyBarRef)
{
	ManComponent_Properties* properties = malloc(sizeof(ManComponent_Properties));
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
	properties->playerWin = false;

	//load texture anim
	properties->TxRunDown	= H3_Texture_Load("assets/Sprites/Man/ManMovefront.png", &properties->TxW, &properties->TxH);
	properties->TxRunUp		= H3_Texture_Load("assets/Sprites/Man/ManMoveBehind.png", &properties->TxW, &properties->TxH);
	properties->TxRunLeft	= H3_Texture_Load("assets/Sprites/Man/ManMoveLeft.png", &properties->TxW, &properties->TxH);
	properties->TxRunRight	= H3_Texture_Load("assets/Sprites/Man/ManMoveRight.png", &properties->TxW, &properties->TxH);
	properties->TxIdleDown	= H3_Texture_Load("assets/Sprites/Man/ManIdleFront.png", &properties->TxW, &properties->TxH);
	properties->TxIdleUp	= H3_Texture_Load("assets/Sprites/Man/ManIdlebehind.png", &properties->TxW, &properties->TxH);
	properties->TxIdleLeft	= H3_Texture_Load("assets/Sprites/Man/ManIdleLeft.png", &properties->TxW, &properties->TxH);
	properties->TxIdleRight	= H3_Texture_Load("assets/Sprites/Man/ManIdleRight.png", &properties->TxW, &properties->TxH);


	properties->direction = 3; //down
	return properties;
}
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(ManComponent,MANCOMPONENT_TYPEID, bool, IsSprint);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ManComponent, MANCOMPONENT_TYPEID, float, slowdown);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ManComponent, MANCOMPONENT_TYPEID, bool, playerWin);