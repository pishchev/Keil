#include <stm32f0xx.h>
#include <time.h>
#include <stdbool.h>

#include "buttons.h"
#include "oscilograph.h"

void Init(void);
void Init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	
	GPIOC->MODER &= ~GPIO_MODER_MODER6;
	GPIOC->MODER &= ~GPIO_MODER_MODER7;
	GPIOC->MODER &= ~GPIO_MODER_MODER8;
	GPIOC->MODER &= ~GPIO_MODER_MODER9;
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;
	
	
	GPIOC->MODER &= ~GPIO_MODER_MODER12;
	GPIOC->MODER |=	GPIO_MODER_MODER12_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER4; 
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1; 
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->MODER &= ~GPIO_MODER_MODER15;
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	
	GPIOA->MODER |= GPIO_MODER_MODER1;
	initADC();
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

void SysTick_Handler(void);
void SysTick_Handler(void) 
{	
	
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
	
}

void SPI2_IRQHandler(void);
void SPI2_IRQHandler(void)
{	
	volatile uint16_t data = SPI2->DR;
	stagingPacket(&packet);
}

static DMA dma;

void  DMA1_Channel1_IRQHandler(void);
void  DMA1_Channel1_IRQHandler(void){    //  DMA ADC1
	
	if (DMA1->ISR & DMA_ISR_HTIF1) {    // Channel 1 Half Transfer flag
		dma.DMA_half=true;
		DMA1->IFCR |= DMA_IFCR_CHTIF1;
	}

	if (DMA1->ISR & DMA_ISR_TCIF1) {    // Channel 1 Transfer Complete flag
		dma.DMA_full=true;
		DMA1->IFCR |= DMA_IFCR_CTCIF1;
	}

}

int main(void)
{
	Init();
	timer_init();
	initSPI();
	initDMA(&dma);
	ConstrPacket(&packet);
	
	for(int i = 0; i < 4; ++i) {
		createButton(&(buttons[i]));
	}

	while(1) 
	{
		DMAEveryTick(&dma, &packet);
	}
			
}
