#ifndef _H3_COMPONENTS_INVENTORYGIRLCOMPONENT_H_
#define _H3_COMPONENTS_INVENTORYGIRLCOMPONENT_H_

#include <h3.h>

#define INVENTORYGIRLCOMPONENT_TYPEID 0x20000010

H3_CAPI_BEGIN_BLOCK
void InventoryGirlComponent_Terminate(void* properties);
void InventoryGirlComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void InventoryGirlComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);
void InventoryGirlComponent_OnTriggerEnter(H3Handle object, SH3Collision collision);
void InventoryGirlComponent_OnTriggerLeave(H3Handle object, H3Handle other);

void* InventoryGirlComponent_CreateProperties(H3Handle* GameScene, H3Handle* EnergyBar, bool IsMan);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(InventoryGirlComponent, H3Handle, ObjSlot3);
H3_CAPI_END_BLOCK

#define INVENTORYGIRLCOMPONENT_CREATE(GAMESCENE,ENERGYBAR,ISMAN)									 \
	(SH3Component) {                                                 \
		.Terminate          = InventoryGirlComponent_Terminate,          \
		.Update             = InventoryGirlComponent_Update,             \
		.Draw				= InventoryGirlComponent_Draw,               \
		.OnTriggerEnter     = InventoryGirlComponent_OnTriggerEnter,     \
		.OnTriggerLeave     = InventoryGirlComponent_OnTriggerLeave,     \
		.isInitialized      = false,                                 \
		.componentType      = INVENTORYGIRLCOMPONENT_TYPEID,             \
		.properties         = InventoryGirlComponent_CreateProperties(GAMESCENE,ENERGYBAR,ISMAN)  \
	}

#endif /* _H3_COMPONENTS_INVENTORYGIRLCOMPONENT_H_ */

