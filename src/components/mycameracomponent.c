#include <components/mycameracomponent.h>

#include <stdlib.h>

typedef struct
{
	H3Handle player;
	float Width;
	float Height;
} MyCameraComponent_Properties;

void MyCameraComponent_Terminate(void* properties)
{
	free(properties);
}

void MyCameraComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	MyCameraComponent_Properties* props = (MyCameraComponent_Properties*)properties;

	//vars
	float playerx;
	float playery;

	//take the position of player and follow the player
	SH3Transform* transformplayer = H3_Object_GetTransform(props->player);
	H3_Transform_GetPosition(transformplayer, &playerx, &playery);
	H3_Object_SetTranslation(object, playerx, playery);

	// Set the vision of the camera
	H3_SetView(h3, transform, props->Width, props->Height);

}

void* MyCameraComponent_CreateProperties(float width, float height, H3Handle player)
{
	MyCameraComponent_Properties* properties = malloc(sizeof(MyCameraComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->Width = width;
	properties->Height = height;
	properties->player = player;

	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW(MyCameraComponent, float, Width);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW(MyCameraComponent, float, Height);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(MyCameraComponent, MYCAMERACOMPONENT_TYPEID, float, Width);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(MyCameraComponent, MYCAMERACOMPONENT_TYPEID, float, Height);
