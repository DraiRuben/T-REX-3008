#include <components/energycomponent.h>

#include <stdlib.h>

typedef struct
{
	H3Handle energyBar;
	H3Handle backBar;
	// 100% energy = 1.0f
	float energy;
} EnergyComponent_Properties;

void EnergyComponent_Terminate(void* properties)
{
	EnergyComponent_Properties* props = (EnergyComponent_Properties*)properties;
	
	if (props->energyBar)
		H3_Texture_Destroy(props->energyBar);

	if (props->backBar)
		H3_Texture_Destroy(props->backBar);
	
	free(properties);
}

void EnergyComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties)
{
	EnergyComponent_Properties* props = (EnergyComponent_Properties*)properties;

	H3_Texture_DrawRectEx(h3, transform, props->backBar,   0.0f, 0.0f, 1.0f, 1.0f, A_Top + A_Left);
	H3_Texture_DrawRectEx(h3, transform, props->energyBar, 0.03f, 0.0f, props->energy, 1.0f, A_Top + A_Left);
}

void EnergyComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	EnergyComponent_Properties* props = (EnergyComponent_Properties*)properties;

	if (props->energy > 0)
		props->energy -= 0.01f * H3_GetDeltaTime();
}

void* EnergyComponent_CreateProperties(H3Handle textureBar, H3Handle textureBackBar)
{
	EnergyComponent_Properties* properties = malloc(sizeof(EnergyComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->energyBar	= textureBar;
	properties->backBar		= textureBackBar;

	properties->energy		= 1.0f;

	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(EnergyComponent, ENERGYCOMPONENT_TYPEID, float, energy);
