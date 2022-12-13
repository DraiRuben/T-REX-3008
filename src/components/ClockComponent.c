#include "components/ClockComponent.h"
#include "components/textcomponent.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
typedef struct
{
	bool IsInitialized;
	bool* IsNewWave;
	//time
	char time[256];
	float minutes;
	float hours;
	SH3TextProperties* textprops;
} ClockComponent_Properties;

void ClockComponent_Terminate(void* properties)
{
	free(properties);
}

void ClockComponent_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {
	ClockComponent_Properties* props = (ClockComponent_Properties*)properties;
	//clock creation
	if (!props->IsInitialized) {
		H3_Object_AddComponent(object, TEXTCOMPONENT_CREATE(props->time, *props->textprops));
		H3_Object_SetTranslation(object, 0, -125);
		props->IsInitialized = true;
	}

	//flow of time
	props->minutes += 0.5*H3_GetDeltaTime();
	if (props->minutes >= 60) {
		if (props->hours == 23) {
			
			props->hours = 0;
		}
		else {
			props->hours += 1;
		}
		*props->IsNewWave = true;
		props->minutes = 0;
	}
	//concatenate hours and min into a char
	snprintf(props->time, 256, "%d:%d", (int)floor(props->hours), (int)floor(props->minutes));

	//adds an extra 0 if needed, e.g if time is 21h and 7 min write 21:07 instead of 21:7
	if ((int)floor(props->hours) >= 0 && (int)floor(props->hours) < 10) {
		snprintf(props->time, 256, "0%d:%d", (int)floor(props->hours), (int)floor(props->minutes));
	}
	if ((int)floor(props->minutes) >= 0 && (int)floor(props->minutes) < 10) {
		snprintf(props->time, 256, "%d:0%d", (int)floor(props->hours), (int)floor(props->minutes));
	}
	if ((int)floor(props->minutes) >= 0 && (int)floor(props->minutes) < 10 && (int)floor(props->hours) >= 0 && (int)floor(props->hours) < 10) {
		snprintf(props->time, 256, "0%d:0%d", (int)floor(props->hours), (int)floor(props->minutes));
	}
	//enter time in textcomp
	TextComponent_SetTextEx(object, props->time);
}

void* ClockComponent_CreateProperties(SH3TextProperties* textprops, bool* IsNewWave)
{
	ClockComponent_Properties* properties = malloc(sizeof(ClockComponent_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");
	properties->minutes = 30;
	properties->IsNewWave = IsNewWave;
	properties->hours = 20;
	properties->IsInitialized = false;
	properties->textprops = textprops;
	return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ClockComponent, CLOCKCOMPONENT_TYPEID, float, hours);