#include "buttons.h"

void createButton(Button* button)
{
	button->state = false;
	button->lastState = false;
	button->pressDown = false;
	button->checked = false;
	button->timeAfterPress = 0;
}

void ButtonEvent(Button* button, bool state)
{	
	if (button->pressDown)
	{
		button->state = false;
		if (!state)
		{
			button->pressDown = false;
		}
	}
	
	if (!button->pressDown && state)
	{
		button->pressDown = true;
		button->state = true;
	}
}
