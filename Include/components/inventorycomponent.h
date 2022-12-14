#ifndef _H3_COMPONENTS_INVENTORYCOMPONENT_H_
#define _H3_COMPONENTS_INVENTORYCOMPONENT_H_

#include <h3.h>

#define INVENTORYCOMPONENT_TYPEID 0x20000002

H3_CAPI_BEGIN_BLOCK
void InventoryComponent_Terminate(void* properties);
void InventoryComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void InventoryComponent_Draw(H3Handle h3, SH3Transform* transform, void* properties);
void InventoryComponent_OnTriggerEnter(H3Handle object, SH3Collision collision);
void InventoryComponent_OnTriggerLeave(H3Handle object, H3Handle other);

void* InventoryComponent_CreateProperties(H3Handle* GameScene,H3Handle* EnergyBar);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(InventoryComponent, H3Handle, ObjSlot2);
H3_CAPI_END_BLOCK

#define INVENTORYCOMPONENT_CREATE(GAMESCENE,ENERGYBAR)									 \
	(SH3Component) {                                                 \
		.Terminate          = InventoryComponent_Terminate,          \
		.Update             = InventoryComponent_Update,             \
		.Draw				= InventoryComponent_Draw,               \
		.OnTriggerEnter     = InventoryComponent_OnTriggerEnter,     \
		.OnTriggerLeave     = InventoryComponent_OnTriggerLeave,     \
		.isInitialized      = false,                                 \
		.componentType      = INVENTORYCOMPONENT_TYPEID,             \
		.properties         = InventoryComponent_CreateProperties(GAMESCENE,ENERGYBAR)  \
	}

#endif /* _H3_COMPONENTS_INVENTORYCOMPONENT_H_ */
