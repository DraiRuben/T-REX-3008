#include "components/AisleSpawnerComponent.h"
#include "components/spritecomponent.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
	int aisleID;
	char schoolAisle[100];
	char uselessAisle[100];
	char monsterAisle[100];
	bool IsInitialized;
	char AisleTempName[256];
	int AisleTempIndex;
	//ref to scene
	H3Handle* GameScene;

} AisleSpawnerComponent_Properties;


void AisleSpawnerComponent_Terminate(void* properties)
{
	free(properties);
}

int limit = 100;
bool IsMonsterInitialized = false;
void AisleSpawnerComponentUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	AisleSpawnerComponent_Properties* props = (AisleSpawnerComponent_Properties*)properties;
	if (!props->IsInitialized) {
		for (int i = 0; i < 5; i++) {
			for (int u = 0; u < 3;u++) {
				props->AisleTempIndex += 1;
				snprintf(props->AisleTempName, 256, "Aisle_%d", props->AisleTempIndex);
				H3Handle Aisle = H3_Object_Create2(*props->GameScene, props->AisleTempName, NULL, 4);
				props->aisleID = rand() % limit;
				if (!(i > 3 && u > 1 && !IsMonsterInitialized)) {
					if (props->aisleID <= 100 && props->aisleID>90) {
						limit -= 10;
						H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->monsterAisle, 0x11));
						IsMonsterInitialized = true;
					}
					else if (props->aisleID <= 90 && props->aisleID>75) {
						H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->schoolAisle, 0x11));
					}
					else if (props->aisleID <= 75) {
						H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->uselessAisle, 0x11));
					}
				}
				else {
					limit -= 10;
					H3_Object_AddComponent(Aisle, SPRITECOMPONENT_CREATE(props->monsterAisle, 0x11));
					IsMonsterInitialized = true;
				}

				if (i < 4) {
					H3_Object_SetTranslation(Aisle, 598 + 288 * u, 1286 + 192 * i);
				}
				else {
					H3_Object_SetTranslation(Aisle, 598 + 288 * u, 2150);

				}
			}
		}
		props->IsInitialized = true;
	}
}


void* AisleSpawnerComponent_CreateProperties(H3Handle* GameScene)
{
	AisleSpawnerComponent_Properties* properties = malloc(sizeof(AisleSpawnerComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	AisleSpawnerComponent_Properties* props = (AisleSpawnerComponent_Properties*)properties;
	sprintf_s(props->uselessAisle,100, "assets/map/rayonpif.png");
	sprintf_s(props->schoolAisle,100, "assets/map/rayonscolaire.png");
	sprintf_s(props->monsterAisle,100, "assets/map/rayonmonster.png");
	properties->AisleTempIndex = 0;
	properties->GameScene = GameScene;
	properties->IsInitialized = false;
	return properties;
}