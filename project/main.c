#include <stm32f0xx.h>
#include <time.h>
#include <stdbool.h>

#include "buttons.h"
#include "spi.h"
#include "numbers.h"
#include "timer.h"

void Init(void);
void Init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN;
	GPIOC->MODER &= ~GPIO_MODER_MODER6;//sbros
	GPIOC->MODER &= ~GPIO_MODER_MODER7;//sbros
	
	GPIOC->MODER |= GPIO_MODER_MODER6_0;
	GPIOC->MODER &= ~GPIO_MODER_MODER7;
	
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR6_1;
	
	GPIOC->MODER &= ~GPIO_MODER_MODER12;
	GPIOC->MODER |=	GPIO_MODER_MODER12_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER4;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1;
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->MODER &= ~GPIO_MODER_MODER15;
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
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
	resetBit(&GPIOC->ODR, GPIO_ODR_6);
	resetBit(&GPIOC->ODR, GPIO_ODR_7);
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

DoublePole val;

void drawPole(Pole pole , int pos);
void drawPole(Pole pole , int pos)
{
	for (int k = 0; k < 8; k ++)
			for (int i =  pos* 4; i < 8; i ++)
				{
						packet.data[k][i] = pole.matrix[k-1*pos][i];
				}
}

void draw(DoublePole poles);
void draw(DoublePole poles)
{
	drawPole(poles.pole0 , 0);
	drawPole(poles.pole1 , 1);
}


int main(void)
{
	Init();
	timer_init();
	initSPI();
	initializeTimer();
	ConstrPacket(&packet);
	
	setNumber(&val,0);
	draw(val);

  int point1;
	
	int num = 0 ;
	
	int send_counter = 0 ;
	
	int counter = 0; 
	
	int tick_needed = 0 ;
	
	while(1) 
	{
		num = 0 ;
		send_counter = 0 ;
		
		setBit(&GPIOC->ODR, GPIO_ODR_6);	
		while (send_counter< 100)
		{send_counter+=1;}			
		resetBit(&GPIOC->ODR, GPIO_ODR_6);
		
		while (!(GPIOC->IDR & GPIO_IDR_7))
		{num+=1; }		
		
		counter +=  num; 		
		tick_needed +=1;
		
		if (tick_needed>100)
		{
			tick_needed = 0 ; 		
			setNumber(&val, counter);
			draw(val);	
			counter = 0;
		}
		
	}
}