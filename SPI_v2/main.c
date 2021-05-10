#include <stm32f0xx.h>
#include <time.h>
#include <stdbool.h>

#include "buttons.h"
#include "spi.h"

void Init(void);
void Init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;
	
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
	SysTick_Config(SystemCoreClock/1000);
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

static Button buttons[4];

static bool tick = 0;
static uint64_t tick_ = 0;

void SysTick_Handler(void);
void SysTick_Handler(void) 
{	
	
	tick_ += 1;
	if (tick_>70)
	{	
		tick = !tick;
		tick_ = 0;
	}
	
	
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
	
	GPIOA->ODR |= GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;

	ButtonEvent(&buttons[0], GPIOA->IDR & GPIO_IDR_4);
	ButtonEvent(&buttons[1], GPIOA->IDR & GPIO_IDR_5);
		
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
	
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR |= GPIO_ODR_12;
	
	ButtonEvent(&buttons[2], GPIOA->IDR & GPIO_IDR_4);
	ButtonEvent(&buttons[3], GPIOA->IDR & GPIO_IDR_5);
	
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
	
	if (!(SPI2->SR & SPI_SR_BSY))
	{
		stagingPacket(&packet);
	}
}

void sleep(unsigned int sec);
void sleep(unsigned int sec)
{
	unsigned int TimerTick = SystemCoreClock / 5000 * sec;
	SysTick->LOAD=TimerTick;
	SysTick->VAL=TimerTick;
	
	SysTick->CTRL =	SysTick_CTRL_CLKSOURCE_Msk |
									SysTick_CTRL_TICKINT_Msk   |
									SysTick_CTRL_ENABLE_Msk;
	
	while(1){
		if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
			break;
		}
	}
}
void drawPoint(int x,int y);
void drawPoint(int x,int y)
{
	if (x< 0)
	{
		x = 7;
	}
	if (x > 7)
	{
		x = 0;
	}
	if (y< 0)
	{
		y = 7;
	}
	if (y > 7)
	{
		y = 0;
	}
	
	packet.data[x][y] = true;
}

void crestx(int x,int y);
void crestx(int x,int y)
{
	for (int ix = -1; ix <= 1 ; ix ++)
	{
		drawPoint(x + ix,y+ ix);
		drawPoint(x + ix,y - ix);
	}
}

void crest(int x,int y);
void crest(int x,int y)
{
	for (int ix = -1 ; ix <= 1 ; ix ++)
	{
		drawPoint(x + ix,y);
	}
	for (int iy = -1 ; iy <= 1 ; iy ++)
	{
		drawPoint(x,y+iy);
	}
}

void crest_anim(int x,int y);
void crest_anim(int x,int y)
{
	if (tick)
		{
			crestx(x,y);
		}
		else 
		{
			crest(x,y);
		}
}



void map_draw(void);
void map_draw()
{
	for ( int k = 2 ; k <8 ; k+=3)
	{
		for (int i = 0 ; i < 8; i ++)
		{
			drawPoint(k,i);
			drawPoint(i,k);
		}
	}
	
}





int main(void)
{
	Init();
	timer_init();
	initSPI();
	ConstrPacket(&packet);
	
	int x = 0;
	int y = 0;
	
	for(int i = 0; i < 4; ++i) {
		createButton(&(buttons[i]));
	}

	while(1) 
	{
		
		if (buttons[3].valueChanged)
		{
			buttons[3].valueChanged = false;
			x = x - 1;
			if ( x<  0)
				x=7;
		}

		if (buttons[2].valueChanged)
		{
			buttons[2].valueChanged = false;
			x = x + 1;
			if ( x > 7)
				x=0;
		}

		if (buttons[0].valueChanged)
		{
			buttons[0].valueChanged = false;
			y = y - 1;
			if ( y<  0)
				y=7;
	  }
		if (buttons[1].valueChanged)
		{
			buttons[1].valueChanged = false;
			y = y + 1;
			if ( y> 7)
				y= 0;
		}
		
		for(int i = 0; i<8;i++)
		for(int j = 0; j<8;j++)
		{
			if ((i == x & j == y)|
					(i == x+1 & j == y)|
					(i == x-1 & j == y)|
					(i == x & j == y+1)|
					(i == x & j == y-1))
			{
				packet.data[i][j] = true;
			}
			else
			{
				packet.data[i][j] = false;
			}
		}
		
	}
}
