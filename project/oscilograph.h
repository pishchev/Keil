#include <stm32f0xx.h>
#include <stdbool.h>
#include "spi.h"

void initADC()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CHSELR |= ADC_CHSELR_CHSEL1;					
	ADC1->CFGR1 |= 0x01 << ADC_CFGR1_RES_Pos; //resolution (1024 points)
	//ADC1->CFGR1 |= ADC_CFGR1_CONT; //continuous conversion mode
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL) {}
	ADC1->CR |= ADC_CR_ADEN;

	while (!(ADC1->ISR & ADC_ISR_ADRDY)) {}
	ADC1->CR |= ADC_CR_ADSTART;
}

void drawOSC(Packet* packet, uint16_t value,int*tick)
{
	//if (*tick> 100)
	{
		*tick = 0;
		uint16_t osc = (value+1)*8/1024;
	
		for(int i = 7; i>=0;i--)
			for(int j = 7; j>=1;j--)
					packet->data[i][j] = packet->data[i][j-1];
	
		
		for(int i = 8; i>=0; i--)
				if (i==osc)
				{
					packet->data[i][0] = true;
				}
				else
				{
					packet->data[i][0] = false;
				}
	}
}

typedef struct _DMA
{
	uint16_t ADC_array[16];
	bool DMA_half;
	bool DMA_full;
}DMA;

void initDMA(DMA* dma)
{
	for(int i=0;i<16;++i)
		dma->ADC_array[i] = 0;
	dma->DMA_full = false;
	dma->DMA_half = false;
	
	ADC1->CFGR1 |= ADC_CFGR1_CONT | ADC_CFGR1_OVRMOD; 
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
	ADC1->CR |= ADC_CR_ADSTART;
	/* The following example is given for the ADC. It can be easily ported on
any peripheral supporting DMA transfer taking of the associated channel
to the peripheral, this must check in the datasheet. */
/* (1) Enable the peripheral clock on DMA */
/* (2) Enable DMA transfer on ADC */
/* (3) Configure the peripheral data register address */
/* (4) Configure the memory address */
/* (5) Configure the number of DMA tranfer to be performs on channel 1 */
/* (6) Configure increment, size and interrupts */
/* (7) Enable DMA Channel 1 */
	RCC->AHBENR |= RCC_AHBENR_DMA1EN; /* (1) */
	//ADC1->CFGR1 |= ADC_CFGR1_DMAEN; /* (2) */
	DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR)); /* (3) */
	DMA1_Channel1->CMAR = (uint32_t)(dma->ADC_array); /* (4) */
	DMA1_Channel1->CNDTR = 16; /* (5) */
	DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0; /* (6) */
	//DMA1_Channel1->CCR |= DMA_CCR_TEIE | DMA_CCR_TCIE ;
	DMA1_Channel1->CCR |= DMA_CCR_PL | DMA_CCR_CIRC;
	DMA1_Channel1->CCR |= DMA_CCR_HTIE|DMA_CCR_TCIE;
	DMA1_Channel1->CCR |= DMA_CCR_EN; /* (7) */
	/* Configure NVIC for DMA */
	/* (1) Enable Interrupt on DMA Channel 1 */
	/* (2) Set priority for DMA Channel 1 */
	NVIC_EnableIRQ(DMA1_Channel1_IRQn); /* (1) */
	//NVIC_SetPriority(DMA1_Channel1_IRQn,0);
}

void DMAEveryTick(DMA* dma, Packet* packet , int*tick)
{
	uint16_t result = 0;
	if (dma->DMA_half) {
			result = dma->ADC_array[0] +  dma->ADC_array[1] + dma->ADC_array[2] + dma->ADC_array[3] + dma->ADC_array[4] + dma->ADC_array[5] + dma->ADC_array[6] + dma->ADC_array[7];
			result = result/8;
			dma->DMA_half = false;
		drawOSC(packet, result , tick);
		}
	if (dma->DMA_full) {
		result = dma->ADC_array[8] +  dma->ADC_array[9] + dma->ADC_array[10] + dma->ADC_array[11] + dma->ADC_array[12] + dma->ADC_array[13] + dma->ADC_array[14] + dma->ADC_array[15];
		result = result/8;
		dma->DMA_full = false;
		drawOSC(packet, result, tick);
	}
}

void initSP(DMA* dma) {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN;

	GPIOA->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	GPIOC->MODER |= GPIO_MODER_MODER12_0;

	GPIOA->MODER &= ~GPIO_MODER_MODER0;

	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1;

	GPIOB->MODER |= GPIO_MODER_MODER15_1 | GPIO_MODER_MODER13_1;
	GPIOA->MODER |= GPIO_MODER_MODER8_0;

	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

	GPIOB->AFR[1] |= (0 << 4 * (15 - 8)) | (0 << 4 * (13 - 8));

	SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA;
	SPI2->CR2 |= SPI_CR2_DS | SPI_CR2_RXNEIE; // 16bit data size

	SPI2->CR1 |= SPI_CR1_SPE;

	NVIC_EnableIRQ(SPI2_IRQn);

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 								//
	GPIOA->MODER |= GPIO_MODER_MODER1;									// Analog mode PA1


	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
	ADC1->CFGR1 |= ADC_CFGR1_CONT | ADC_CFGR1_OVRMOD; 
	
	ADC1->CHSELR |= ADC_CHSELR_CHSEL1;										//Channel 1 of ADC
	ADC1->CFGR1 |= 0x01 << ADC_CFGR1_RES_Pos; //10 bit resolution
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL) {};
	ADC1->CR |= ADC_CR_ADEN;
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
	
	while (!(ADC1->ISR & ADC_ISR_ADRDY)) {};
	
	ADC1->CR |= ADC_CR_ADSTART;
	GPIOC->MODER |= GPIO_MODER_MODER8_1;	
	//GPIOC->AFR[1]	 - default alternative function is 0

	//init DMA
/* (1) Enable the peripheral clock on DMA */
/* (3) Configure the peripheral data register address */
/* (4) Configure the memory address */
/* (5) Configure the number of DMA tranfer to be performs on channel 1 */
/* (6) Configure increment, size and interrupts */
/* (7) Enable DMA Channel 1 */
	RCC->AHBENR |= RCC_AHBENR_DMA1EN; /* (1) */ 
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR)); /* (3) */
	DMA1_Channel1->CMAR = (uint32_t)(dma->ADC_array); /* (4) */
	DMA1_Channel1->CNDTR = 16; /* (5) */
	DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0
		| DMA_CCR_PL | DMA_CCR_CIRC; /* (6) */
		DMA1_Channel1->CCR|=DMA_CCR_HTIE|DMA_CCR_TCIE;
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_SetPriority(DMA1_Channel1_IRQn,0);

}