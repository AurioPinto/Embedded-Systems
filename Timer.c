#include"./library/src/stm32f10x_it.h"
#include"./library/src/stm32f10x_nvic.h"
#include"./library/src/stm32f10x_gpio.h"
#include"./library/src/stm32f10x_flash.h"
#include"./library/src/stm32f10x_rcc.h"
#include"./library/src/stm32f10x_exti.h"
#include"./library/src/stm32f10x_tim.h"
#include "stdio.h"
#include "stm32f10x_lib.h"

ErrorStatus HSEStartUpStatus;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStruct;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

void RCC_Configuration(void){
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if (HSEStartUpStatus == SUCCESS)
{
RCC_HCLKConfig(RCC_SYSCLK_Div1);
RCC_PCLK2Config(RCC_HCLK_Div1);
RCC_PCLK1Config(RCC_HCLK_Div2);
FLASH_SetLatency(FLASH_Latency_2);
FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
RCC_PLLCmd(ENABLE);
while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
while (RCC_GetSYSCLKSource() != 0x08);
}
/* Enable GPIOA~E and AFIO clocks */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
/* TIM1 clock enable */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
/* TIM2 clock enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
/* ADC1 clock enable */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}


int main(void)
{
	RCC_Configuration();

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 35999;
	TIM_TimeBaseStructure.TIM_Prescaler = 999;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	GPIO_SetBits(GPIOC, GPIO_Pin_12);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);

	while(1);
}