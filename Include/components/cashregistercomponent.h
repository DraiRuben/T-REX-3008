#ifndef _H3_COMPONENTS_CASHREGISTERCOMPONENT_H_
#define _H3_COMPONENTS_CASHREGISTERCOMPONENT_H_

#include <h3.h>

#define CASHREGISTERCOMPONENT_TYPEID 0x20000005

H3_CAPI_BEGIN_BLOCK
void CashRegisterComponent_Terminate(void* properties);
void CashRegisterComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void CashRegisterComponent_CollisionEnter(H3Handle object, SH3Collision collision);
void CashRegisterComponent_CollisionLeave(H3Handle object, H3Handle other);

void resetCode(H3Handle digicode);

void* CashRegisterComponent_CreateProperties(H3Handle Digicode, bool* caseIsOpen);

H3_CAPI_END_BLOCK

#define CASHREGISTERCOMPONENT_CREATE(DIGICODE, ADRS_CASHREGISTEROPEN)  /* ADRS = address */           \
	(SH3Component) {                                               \
		.Terminate          = CashRegisterComponent_Terminate,             \
		.Update             = CashRegisterComponent_Update,                \
		.OnCollisionEnter   = CashRegisterComponent_CollisionEnter,        \
		.OnCollisionLeave   = CashRegisterComponent_CollisionLeave,        \
		.isInitialized      = false,                               \
		.componentType      = CASHREGISTERCOMPONENT_TYPEID,                \
		.properties         = CashRegisterComponent_CreateProperties(DIGICODE, ADRS_CASHREGISTEROPEN) \
	}

#endif /* _H3_COMPONENTS_CASHREGISTERCOMPONENT_H_ */
