#include <components/digicodecomponent.h>

#include <components/textcomponent.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	bool isInitialized;
	H3Handle camera;
	H3Handle self;
	uint32_t w, h;
	float digicodeX;
	float digicodeY;
	float widthBtn;
	float heightBtn;

	int realCode;
	int code; //temporairy
	int whichNumb;
	H3Handle firstNumb;
	H3Handle secondNumb;
	H3Handle thirdNumb;
	H3Handle fourthNumb;

	H3Handle btn7, btn8, btn9;
	H3Handle btn4, btn5, btn6;
	H3Handle btn1, btn2, btn3;
	H3Handle btnClose, btn0, btnCheck;
} DigicodeComponent_Properties;

void DigicodeComponent_Terminate(void* properties)
{
	DigicodeComponent_Properties* props = (DigicodeComponent_Properties*)properties;

	H3_Texture_Destroy(props->btnClose);
	H3_Texture_Destroy(props->btnCheck);
	H3_Texture_Destroy(props->btn0);
	H3_Texture_Destroy(props->btn1);
	H3_Texture_Destroy(props->btn2);
	H3_Texture_Destroy(props->btn3);
	H3_Texture_Destroy(props->btn4);
	H3_Texture_Destroy(props->btn5);
	H3_Texture_Destroy(props->btn6);
	H3_Texture_Destroy(props->btn7);
	H3_Texture_Destroy(props->btn8);
	H3_Texture_Destroy(props->btn9);

	free(properties);
}

void DigicodeComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties)
{
	DigicodeComponent_Properties* props = (DigicodeComponent_Properties*)properties;

	//draw btn
	if (myButton(h3, props->btnClose, (props->digicodeX - 32), (props->digicodeY + 64), properties)) {
		props->firstNumb	= props->btn0;
		props->secondNumb	= props->btn0;
		props->thirdNumb	= props->btn0;
		props->fourthNumb	= props->btn0;
		props->whichNumb	= 1;
		props->realCode = 0;
		props->code = 0;
		H3_Object_SetEnabled(props->self, false);
	}
	if (myButton(h3, props->btnCheck, (props->digicodeX + 32), (props->digicodeY + 64), properties)) {
		props->firstNumb	= props->btn0;
		props->secondNumb	= props->btn0;
		props->thirdNumb	= props->btn0;
		props->fourthNumb	= props->btn0;
		props->whichNumb	= 1;
		props->realCode = props->code;
		printf("code : %d\n", props->realCode);
		props->code = 0;
	}
	if (myButton(h3, props->btn0, (props->digicodeX     ), (props->digicodeY + 64), properties)) {
		addToCode(props->btn0, 0, properties);
	}
	if (myButton(h3, props->btn1, (props->digicodeX - 32), (props->digicodeY + 32), properties)) {
		addToCode(props->btn1, 1, properties);
	}
	if (myButton(h3, props->btn2, (props->digicodeX     ), (props->digicodeY + 32), properties)) {
		addToCode(props->btn2, 2, properties);
	}
	if (myButton(h3, props->btn3, (props->digicodeX + 32), (props->digicodeY + 32), properties)) {
		addToCode(props->btn3, 3, properties);
	}
	if (myButton(h3, props->btn4, (props->digicodeX - 32), (props->digicodeY     ), properties)) {
		addToCode(props->btn4, 4, properties);
	}
	if (myButton(h3, props->btn5, (props->digicodeX     ), (props->digicodeY     ), properties)) {
		addToCode(props->btn5, 5, properties);
	}
	if (myButton(h3, props->btn6, (props->digicodeX + 32), (props->digicodeY     ), properties)) {
		addToCode(props->btn6, 6, properties);
	}
	if (myButton(h3, props->btn7, (props->digicodeX - 32), (props->digicodeY - 32), properties)) {
		addToCode(props->btn7, 7, properties);
	}
	if (myButton(h3, props->btn8, (props->digicodeX     ), (props->digicodeY - 32), properties)) {
		addToCode(props->btn8, 8, properties);
	}
	if (myButton(h3, props->btn9, (props->digicodeX + 32), (props->digicodeY - 32), properties)) {
		addToCode(props->btn9, 9, properties);
	}

	//draw code
	H3_Texture_Draw(h3, (props->digicodeX - 13 - 30), (props->digicodeY - 74), props->firstNumb, 0x22);
	H3_Texture_Draw(h3, (props->digicodeX - 13 - 10), (props->digicodeY - 74), props->secondNumb, 0x22);
	H3_Texture_Draw(h3, (props->digicodeX - 13 + 10), (props->digicodeY - 74), props->thirdNumb, 0x22);
	H3_Texture_Draw(h3, (props->digicodeX - 13 + 30), (props->digicodeY - 74), props->fourthNumb, 0x22);
}

void DigicodeComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	DigicodeComponent_Properties* props = (DigicodeComponent_Properties*)properties;

	if (!props->isInitialized){
		props->self = object;
		props->isInitialized = true;
	}
	H3_Transform_GetPosition(transform, &props->digicodeX, &props->digicodeY);
}

/*----------------------------------------------------------------*/

bool myButton(H3Handle h3, H3Handle texture, float x, float y, void* properties)
{
	DigicodeComponent_Properties* props = (DigicodeComponent_Properties*)properties;
	
	//draw btn
	float midW = props->widthBtn / 2;
	float midH = props->heightBtn / 2;
	H3_Texture_Draw(h3, (x - midW), (y - midH), texture, 0x22);

	//is click on button
	if (H3_Input_IsMouseBtnPressed(MB_Left))
	{
		//get mouse pos
		int mx, my;
		H3_Input_GetMousePos(h3, &mx, &my);
		mx /= 4;
		my /= 4;

		//get camera data
		float camX, camY;
		H3_Transform_GetPosition(H3_Object_GetTransform(props->camera), &camX, &camY);

		//btn pos	 = (btn pos - cam offset)
		float btnX = x - (camX - 240);
		float btnY = y - (camY - 135);

		//btn Close
		if (mx >= (btnX - midW) && my >= (btnY - midH) && mx <= (btnX + midW) && my <= (btnY + midH))
			return true;
	}

	return false;
}

void addToCode(H3Handle NumTexture, int number, void* properties)
{
	DigicodeComponent_Properties* props = (DigicodeComponent_Properties*)properties;

	switch (props->whichNumb)
	{
	case 1:
		//reset
		props->code = 0;
		props->secondNumb	= props->btn0;
		props->thirdNumb	= props->btn0;
		props->fourthNumb	= props->btn0;

		//set new number
		props->firstNumb	= NumTexture;
		props->whichNumb	= 2;
		props->code += number * 1000;
		break;
	case 2:
		props->secondNumb = NumTexture;
		props->whichNumb = 3;
		props->code += number * 100;
		break;
	case 3:
		props->thirdNumb = NumTexture;
		props->whichNumb = 4;
		props->code += number * 10;
		break;
	case 4:
		props->fourthNumb = NumTexture;
		props->whichNumb = 1;
		props->code += number;
		break;
	default:
		break;
	}
}

/*----------------------------------------------------------------*/

void* DigicodeComponent_CreateProperties(H3Handle cameraRef)
{
	DigicodeComponent_Properties* properties = malloc(sizeof(DigicodeComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->camera		= cameraRef;

	properties->btnClose = H3_Texture_Load("assets/Sprites/DigiBtnEmpty.png", &properties->w, &properties->h);
	properties->btnCheck = H3_Texture_Load("assets/Sprites/DigiBtnEmpty.png", &properties->w, &properties->h);
	properties->btn0 = H3_Texture_Load("assets/Sprites/0.png", &properties->w, &properties->h);
	properties->btn1 = H3_Texture_Load("assets/Sprites/1.png", &properties->w, &properties->h);
	properties->btn2 = H3_Texture_Load("assets/Sprites/2.png", &properties->w, &properties->h);
	properties->btn3 = H3_Texture_Load("assets/Sprites/3.png", &properties->w, &properties->h);
	properties->btn4 = H3_Texture_Load("assets/Sprites/4.png", &properties->w, &properties->h);
	properties->btn5 = H3_Texture_Load("assets/Sprites/5.png", &properties->w, &properties->h);
	properties->btn6 = H3_Texture_Load("assets/Sprites/6.png", &properties->w, &properties->h);
	properties->btn7 = H3_Texture_Load("assets/Sprites/7.png", &properties->w, &properties->h);
	properties->btn8 = H3_Texture_Load("assets/Sprites/8.png", &properties->w, &properties->h);
	properties->btn9 = H3_Texture_Load("assets/Sprites/9.png", &properties->w, &properties->h);
	
	properties->code		= 0;
	properties->realCode	= 0;
	properties->whichNumb	= 1;
	properties->firstNumb	= properties->btn0;
	properties->secondNumb	= properties->btn0;
	properties->thirdNumb	= properties->btn0;
	properties->fourthNumb	= properties->btn0;
	
	properties->isInitialized = false;
	properties->widthBtn = 26;
	properties->heightBtn = 26;

	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, DIGICODECOMPONENT_TYPEID, H3Handle, firstNumb);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, DIGICODECOMPONENT_TYPEID, H3Handle, secondNumb);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, DIGICODECOMPONENT_TYPEID, H3Handle, thirdNumb);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, DIGICODECOMPONENT_TYPEID, H3Handle, fourthNumb);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(DigicodeComponent, DIGICODECOMPONENT_TYPEID, H3Handle, btn0);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, DIGICODECOMPONENT_TYPEID, int, code);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, DIGICODECOMPONENT_TYPEID, int, realCode);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, DIGICODECOMPONENT_TYPEID, int, whichNumb);
