#include <components/cashregistercomponent.h>
#include <components/mancomponent.h>
#include <components/digicodecomponent.h>
#include <components/spritecomponent.h>
#include <components/collectablecomponent.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	H3Handle* Player;
	H3Handle* GameScene;
	H3Handle digicode;
	H3Handle OpenSFX;
	bool* isOpen;
} CashRegisterComponent_Properties;

void CashRegisterComponent_Terminate(void* properties)
{
	free(properties);
}
float px, py;
void CashRegisterComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	CashRegisterComponent_Properties* props = (CashRegisterComponent_Properties*)properties;

	//open cashregister
	if (!*props->isOpen && DigicodeComponent_GetrealCodeEx(props->digicode) == 904)
	{
		*props->isOpen = true;
		resetCode(props->digicode);
		H3_Sound_Play(props->OpenSFX, 80, false);
		H3Handle Key = H3_Object_Create2(*props->GameScene,"Key",NULL,4);
		H3_Object_AddComponent(Key, SPRITECOMPONENT_CREATE("assets/Objects/BlackKey.png", 0x22));
		H3_Object_AddComponent(Key, COLLECTABLECOMPONENT_CREATE(9, 1,NULL));
		H3_Object_EnablePhysics(Key, H3_BOX_COLLIDER(CDT_Dynamic, 12, 6, 0x11, true));
		H3_Transform_GetPosition(H3_Object_GetTransform(*props->Player), &px, &py);
		H3_Object_SetTranslation(Key, px-50, py);
		//unshow digicode
		H3_Object_SetEnabled(props->digicode, false);
	}
}

void CashRegisterComponent_CollisionEnter(H3Handle object, SH3Collision collision)
{
	SH3Component* component = H3_Object_GetComponent(object, CASHREGISTERCOMPONENT_TYPEID);
	CashRegisterComponent_Properties* props = (CashRegisterComponent_Properties*)component->properties;

	if (!*props->isOpen && collision.other != NULL && (H3_Object_HasComponent(collision.other, MANCOMPONENT_TYPEID) || H3_Object_HasComponent(collision.other, MANCOMPONENT_TYPEID)))
	{
		H3_Object_SetEnabled(props->digicode, true);
	}
}

void CashRegisterComponent_CollisionLeave(H3Handle object, H3Handle other)
{
	SH3Component* component = H3_Object_GetComponent(object, CASHREGISTERCOMPONENT_TYPEID);
	CashRegisterComponent_Properties* props = (CashRegisterComponent_Properties*)component->properties;

	if (!*props->isOpen && other != NULL && (H3_Object_HasComponent(other, MANCOMPONENT_TYPEID)|| H3_Object_HasComponent(other, MANCOMPONENT_TYPEID)))
	{
		resetCode(props->digicode);
		
		//unshow digicode
		H3_Object_SetEnabled(props->digicode, false);
	}
}

/*----------------------------------------------------*/

void resetCode(H3Handle digicode)
{
	//reset code
	H3Handle reset = DigicodeComponent_Getbtn0Ex(digicode);
	DigicodeComponent_SetfirstNumbEx	(digicode, reset);
	DigicodeComponent_SetsecondNumbEx	(digicode, reset);
	DigicodeComponent_SetthirdNumbEx	(digicode, reset);
	DigicodeComponent_SetfourthNumbEx	(digicode, reset);
	DigicodeComponent_SetwhichNumbEx	(digicode, 1);
	DigicodeComponent_SetcodeEx			(digicode, 0);
	DigicodeComponent_SetrealCodeEx		(digicode, 0);
}
/*----------------------------------------------------*/

void* CashRegisterComponent_CreateProperties(H3Handle Digicode, bool* cashregisterIsOpen, H3Handle* GameScene, H3Handle* Player)
{
	CashRegisterComponent_Properties* properties = malloc(sizeof(CashRegisterComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->Player = Player;
	properties->GameScene = GameScene;
	properties->digicode = Digicode;
	properties->OpenSFX = H3_Sound_Load("assets/SFX/CashRegisterSFX.wav");
	//properties->isOpen = false;
	properties->isOpen = cashregisterIsOpen;

	return properties;
}