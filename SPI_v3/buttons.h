#pragma once

#include <stm32f0xx.h>
#include <stdbool.h>

typedef struct _Button
{
	bool currentTrueState; 
	bool currentToggledState; 
	bool valueChanged;
	
	bool nowRawState; 
	bool wasFrontUp;
	bool wasFrontDown; 
	
	
	int countTicksAfterLastToggling; 
	int ticksDisabling;
}Button;

void createButton(Button* button);
void ButtonEvent(Button* button, bool rawState);

