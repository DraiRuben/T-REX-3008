#include <components/cashregistercomponent.h>

#include <components/playercomponent.h>
#include <components/digicodecomponent.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	H3Handle digicode;
	bool* isOpen;
} CashRegisterComponent_Properties;

void CashRegisterComponent_Terminate(void* properties)
{
	free(properties);
}

void CashRegisterComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	CashRegisterComponent_Properties* props = (CashRegisterComponent_Properties*)properties;

	//open cashregister
	if (!*props->isOpen && DigicodeComponent_GetrealCodeEx(props->digicode) == 904)
	{
		*props->isOpen = true;
		resetCode(props->digicode);

		//unshow digicode
		H3_Object_SetEnabled(props->digicode, false);
	}
}

void CashRegisterComponent_CollisionEnter(H3Handle object, SH3Collision collision)
{
	SH3Component* component = H3_Object_GetComponent(object, CASHREGISTERCOMPONENT_TYPEID);
	CashRegisterComponent_Properties* props = (CashRegisterComponent_Properties*)component->properties;

	if (!*props->isOpen && collision.other != NULL && H3_Object_HasComponent(collision.other, PLAYERCOMPONENT_TYPEID))
	{
		H3_Object_SetEnabled(props->digicode, true);
	}
}

void CashRegisterComponent_CollisionLeave(H3Handle object, H3Handle other)
{
	SH3Component* component = H3_Object_GetComponent(object, CASHREGISTERCOMPONENT_TYPEID);
	CashRegisterComponent_Properties* props = (CashRegisterComponent_Properties*)component->properties;

	if (!*props->isOpen && other != NULL && H3_Object_HasComponent(other, PLAYERCOMPONENT_TYPEID))
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

void* CashRegisterComponent_CreateProperties(H3Handle Digicode, bool* cashregisterIsOpen)
{
	CashRegisterComponent_Properties* properties = malloc(sizeof(CashRegisterComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->digicode = Digicode;
	//properties->isOpen = false;
	properties->isOpen = cashregisterIsOpen;

	return properties;
}