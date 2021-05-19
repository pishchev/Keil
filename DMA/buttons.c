#include "buttons.h"

void createButton(Button* button)
{
	button->currentToggledState = false;
	button->currentTrueState = false;
	button->nowRawState = false;
	button->wasFrontDown = false;
	button->wasFrontUp = false;
	button->valueChanged = false;
	
	button->countTicksAfterLastToggling = 0;
	button->ticksDisabling = 10;
}

void ButtonEvent(Button* button, bool rawState)
{	
	button->valueChanged = false;
	if (button->nowRawState == false && rawState == true)
	{
			button->wasFrontUp = true;
	}
	else
	{
			button->wasFrontUp = false;
	}
	
	if (button->nowRawState == true && rawState == false)
	{
			button->wasFrontDown = true;
	}
	else
	{
			button->wasFrontDown = false;
	}
	
	button->nowRawState = rawState;
	
	if (!button->wasFrontUp && !button->wasFrontDown)
	{
	   button->countTicksAfterLastToggling++;
		if (button->countTicksAfterLastToggling > button->ticksDisabling)
		{
			button->countTicksAfterLastToggling = button->ticksDisabling;
		}
	}
	else
	{
		button->countTicksAfterLastToggling = 0;
	}
	
	if (button->countTicksAfterLastToggling >= button->ticksDisabling)
	{
		if (button->currentTrueState != button->nowRawState)
		{
			button->currentTrueState = button->nowRawState;
			
			if (button->currentTrueState){
				button->currentToggledState = !button->currentToggledState;
				button->valueChanged = true;
			}
		}
	}
}
