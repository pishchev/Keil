#pragma once

#include <stm32f0xx.h>
#include <stdbool.h>

typedef struct _Button
{
	bool currentTrueState; //state of button without drebezg contactov
	bool currentToggledState; //state of button without drebezg contactov with toggling logic
	bool valueChanged;//is true value was changed from low to high in last checking
	
	bool nowRawState; //just state in now (dt), very raw data, useless
	bool wasFrontUp; //is previous raw state with current raw state generate front up 
	bool wasFrontDown; //is previous raw state with current raw state generate front down
	
	
	int countTicksAfterLastToggling; //ticks after last changing state (Chenging state can be made only when this one greater then ticksDisabling)
	int ticksDisabling; //tick time of disabling (drebezg)
}Button;

void createButton(Button* button);
void ButtonEvent(Button* button, bool rawState);

