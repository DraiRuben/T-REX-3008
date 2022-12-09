#include <components/collectablecomponent.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	bool isDoneOnce;
	bool isInHand;
} CollectableComponent_Properties;

void CollectableComponent_Terminate(void* properties)
{
	free(properties);
}

void CollectableComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
	CollectableComponent_Properties* props = (CollectableComponent_Properties*)properties;

	//reset rotation
	if (props->isInHand)
	{
		if (!props->isDoneOnce)
		{
			props->isDoneOnce = true;
			H3_Object_SetRotation(object, 0.0f);
		}
	}
	else
	{
		props->isDoneOnce = false;
	}
}

void* CollectableComponent_CreateProperties()
{
	CollectableComponent_Properties* properties = malloc(sizeof(CollectableComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->isInHand = false;
	properties->isDoneOnce = false;

	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(CollectableComponent, COLLECTABLECOMPONENT_TYPEID, bool, isInHand);