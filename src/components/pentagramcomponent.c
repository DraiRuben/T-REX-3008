#include "components/PentagramComponent.h"
#include "components/collectablecomponent.h"
#include "components/inventorycomponent.h"

#include <components/spritecomponent.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
	H3Handle ActivatedSprite;
	bool IsActivated;
} PentagramComponent_Properties;


void PentagramComponent_Terminate(void* properties)
{
	free(properties);
}

void PentagramComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	PentagramComponent_Properties* props = (PentagramComponent_Properties*)properties;
	

}


void* PentagramComponent_CreateProperties(const char* path)
{
	PentagramComponent_Properties* properties = malloc(sizeof(PentagramComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	uint32_t w, h;
	properties->IsActivated = false;
	properties->ActivatedSprite = H3_Texture_Load(path,&w,&h);
	return properties;
}
float vx, vy;
//changes sprite when in contact with meat
void PentagramTriggerEnter(H3Handle object, SH3Collision obj_id) {
	SH3Component* component = H3_Object_GetComponent(object, PENTAGRAMCOMPONENT_TYPEID);
	PentagramComponent_Properties* props = (PentagramComponent_Properties*)(component->properties);
	if (obj_id.other != NULL) {
		if (H3_Object_HasComponent(obj_id.other, COLLECTABLECOMPONENT_TYPEID)) {
			if (CollectableComponent_GettypeEx(obj_id.other) == 8) {
				props->IsActivated = true;
				SpriteComponent_SetTextureEx(object, props->ActivatedSprite);
				H3_Object_Destroy(obj_id.other, false);
			}
		}
	}
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(PentagramComponent, PENTAGRAMCOMPONENT_TYPEID, bool, IsActivated);