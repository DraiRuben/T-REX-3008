#ifndef _H3_COMPONENTS_CASECOMPONENT_H_
#define _H3_COMPONENTS_CASECOMPONENT_H_

#include <h3.h>

#define CASECOMPONENT_TYPEID 0x20000005

H3_CAPI_BEGIN_BLOCK
void CaseComponent_Terminate(void* properties);
void CaseComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void CaseComponent_CollisionEnter(H3Handle object, SH3Collision collision);
void CaseComponent_CollisionLeave(H3Handle object, H3Handle other);

void resetCode(H3Handle digicode);

void* CaseComponent_CreateProperties(H3Handle Digicode, bool* caseIsOpen);

H3_CAPI_END_BLOCK

#define CASECOMPONENT_CREATE(DIGICODE, ADRS_CASEOPEN)  /* ADRS = address */           \
	(SH3Component) {                                               \
		.Terminate          = CaseComponent_Terminate,             \
		.Update             = CaseComponent_Update,                \
		.OnCollisionEnter   = CaseComponent_CollisionEnter,        \
		.OnCollisionLeave   = CaseComponent_CollisionLeave,        \
		.isInitialized      = false,                               \
		.componentType      = CASECOMPONENT_TYPEID,                \
		.properties         = CaseComponent_CreateProperties(DIGICODE, ADRS_CASEOPEN) \
	}

#endif /* _H3_COMPONENTS_CASECOMPONENT_H_ */
