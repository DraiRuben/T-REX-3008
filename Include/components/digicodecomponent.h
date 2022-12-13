#ifndef _H3_COMPONENTS_DIGICODECOMPONENT_H_
#define _H3_COMPONENTS_DIGICODECOMPONENT_H_

#include <h3.h>

#define DIGICODECOMPONENT_TYPEID 0x20000006

H3_CAPI_BEGIN_BLOCK
void DigicodeComponent_Terminate(void* properties);
void DigicodeComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void DigicodeComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);

bool myButton(H3Handle h3, H3Handle texture, float x, float y, void* properties);
void addToCode(H3Handle NumTexture, int number, void* properties);

void* DigicodeComponent_CreateProperties(H3Handle cameraRef);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, H3Handle, firstNumb);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, H3Handle, secondNumb);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, H3Handle, thirdNumb);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, H3Handle, fourthNumb);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(DigicodeComponent, H3Handle, btn0);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, int, code);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, int, realCode);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(DigicodeComponent, int, whichNumb);
H3_CAPI_END_BLOCK

#define DIGICODECOMPONENT_CREATE(CAMERA)                           \
	(SH3Component) {                                               \
		.Terminate          = DigicodeComponent_Terminate,         \
		.Update             = DigicodeComponent_Update,            \
		.Draw               = DigicodeComponent_Draw,              \
		.isInitialized      = false,                               \
		.componentType      = DIGICODECOMPONENT_TYPEID,            \
		.properties         = DigicodeComponent_CreateProperties(CAMERA) \
	}

#endif /* _H3_COMPONENTS_DIGICODECOMPONENT_H_ */
