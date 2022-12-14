#include <components/collectablecomponent.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	int durability;
	int type;
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

	//reset rotation when is in hand
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
	if (props->durability <= 0) {
		H3_Object_Destroy(object, false);
	}
}

void* CollectableComponent_CreateProperties(int type, int durability)
{
	CollectableComponent_Properties* properties = malloc(sizeof(CollectableComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->type = type;
	properties->durability = durability;
	properties->isInHand = false;
	properties->isDoneOnce = false;

	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(CollectableComponent, COLLECTABLECOMPONENT_TYPEID, bool, isInHand);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(CollectableComponent, COLLECTABLECOMPONENT_TYPEID, int, type);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(CollectableComponent, COLLECTABLECOMPONENT_TYPEID, int, durability);

//types:
//type 1 = monsterAisle
//type 2 = monsterCan
//type 3 = bookAisle
//type 4 = book
//type 5 = bakeryAisle
//type 6 = B A G U E T T E
//type 7 = meatAisle
//type 8 = Meat
//type 9 = Black Key
//type 10 = fishAisle
//type 11 = fish