#pragma once

#include <stm32f0xx.h>
#include <stdbool.h>

static const uint32_t waitingPeriod = 10;
static const uint32_t checkPeriod = 10;

typedef struct _Button
{
	bool state;
	bool lastState;
	
	bool pressDown;
	bool checked;
	
	uint32_t timeAfterPress;
	uint32_t timeAfterChanges;
}Button;

void createButton(Button* button);
void ButtonEvent(Button* button, bool state);

