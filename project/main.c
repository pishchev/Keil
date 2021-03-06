#include <stm32f0xx.h>
#include <time.h>
#include <stdbool.h>

#include "buttons.h"
#include "spi.h"
#include "numbers.h"
#include "timer.h"
#include "transfer.h"

void Init(void);
void Init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN;
	
	GPIOC->MODER &= ~GPIO_MODER_MODER6;//sbros
	GPIOC->MODER &= ~GPIO_MODER_MODER7;//sbros
	GPIOC->MODER &= ~GPIO_MODER_MODER8;
	GPIOC->MODER &= ~GPIO_MODER_MODER9;
	
	GPIOC->MODER |= GPIO_MODER_MODER6_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	
	GPIOC->MODER &= ~GPIO_MODER_MODER7;
	
}
void timer_init(void);
void timer_init()
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/2000);
}


void setBit(volatile uint32_t* bit, uint32_t value);
void setBit(volatile uint32_t* bit, uint32_t value) 
{
	*bit |= value;
}

void resetBit(volatile uint32_t* bit, uint32_t value);
void resetBit(volatile uint32_t* bit, uint32_t value)
{
	*bit &= ~value;
}

void resetAll(void);
void resetAll()
{
	resetBit(&GPIOC->ODR, GPIO_ODR_8);
	resetBit(&GPIOC->ODR, GPIO_ODR_9);
}


static Packet packet;

static int tick = 0;

void SysTick_Handler(void);
void SysTick_Handler(void) 
{	
	tick+=1;
}

void SPI2_IRQHandler(void);
void SPI2_IRQHandler(void)
{	
	volatile uint16_t data = SPI2->DR;
	stagingPacket(&packet);
}

void drawPole(Pole pole , int pos);
void drawPole(Pole pole , int pos)
{
	for (int k = 0; k < 8; k ++)
			for (int i =  pos* 4; i < 8; i ++)
				{
						packet.data[k][i] = pole.matrix[k-1*pos][i];
				}
}

void draw(int num);
void draw(int num)
{
	
	DoublePole val;

	setNumber(&val ,num);
	drawPole(val.pole0 , 0);
	drawPole(val.pole1 , 1);
}

void sendBit(int sendTime);
void sendBit(int sendTime)
{
	setBit(&GPIOC->ODR, GPIO_ODR_6);	
	int timePoint = timer.counter;
	while (timer.counter - timePoint < sendTime)
	{
		
	}
	resetBit(&GPIOC->ODR, GPIO_ODR_6);	
}

static int ticks = 0;

void getBit();
void getBit()
{
	ticks = 0;
	while (!(GPIOC->IDR & GPIO_IDR_7))
	{
	}		
	while (GPIOC->IDR & GPIO_IDR_7)
	{
		ticks ++;
		setBit(&GPIOC->ODR, GPIO_ODR_8);
	}		
	resetBit(&GPIOC->ODR, GPIO_ODR_8);
}

void blicks(bool* blick)
{
	if (*blick)
		{
			setBit(&GPIOC->ODR, GPIO_ODR_9);
		}
		else
		{
			resetBit(&GPIOC->ODR, GPIO_ODR_9);
		}
		*blick = !*blick;
}

int tickToTime(int tick , int a , int c)
{
	return tick/2*a + c;
}

int main(void)
{
	Init();
	timer_init();
	initSPI();
	initializeTimer();
	ConstrPacket(&packet);
	
	ConstrTransfer(&transfer, false);
	
	int timeToDraw = 0;
	
	bool blick = false;
	
	int time_point = 0;
	
	int mean_count =100;
	
	int current_value = 0;
	
	while(1) 
	{
		//blicks(&blick);
		if (transfer.isTransmit)
		{
			int sum = 0;
		
			for (int i = 0 ; i < mean_count ; i ++ )
			{
					timer.counter = 0;
					sendBit(1000);
					time_point =  timer.counter;
					getBit();
					sum+= timer.counter-time_point;
			
			}
		
			int value = tickToTime(sum/mean_count,1,-14);
			transfer.data = value;
			transmitMessage(&transfer);
			draw(value);
		}
		else
		{
			receiveMessage(&transfer);
			if (current_value != transfer.data)
			{
				draw(transfer.data);
				current_value = transfer.data;
			}	
		}
		
	}
}