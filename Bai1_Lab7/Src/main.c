
/*******************************************************************************
 *				 _ _                                             _ _
				|   |                                           (_ _)
				|   |        _ _     _ _   _ _ _ _ _ _ _ _ _ _   _ _
				|   |       |   |   |   | |    _ _     _ _    | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |_ _ _  |   |_ _|   | |   |   |   |   |   | |   |
				|_ _ _ _ _| |_ _ _ _ _ _| |_ _|   |_ _|   |_ _| |_ _|
								(C)2022 Lumi
 * Copyright (c) 2022
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File name: main.h
 *
 * Description:
 *
 *
 * Last Changed By:  $Author: dungpk $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $June 1, 2022
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/

/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
#include "stm32f401re_rcc.h"
#include "stm32f401re_gpio.h"
#include "stm32f401re_tim.h"
#include "typedefs.h"
/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define LED_GREEN_PORT			GPIOA
#define LED_GREEN_PIN			GPIO_Pin_5
#define LED_GREEN				5

#define GPIO_PIN_SET			1
#define GPIO_PIN_RESET			0
#define DELAY_MS_1S				999
#define TIM_PRESCACLE			83
#define TIM_PERIOD				999

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

static void_t led1Control_Init(void_t);
static void_t ledControl_SetState(GPIO_TypeDef *GPIOx,uint8_t GPIO_PIN,uint8_t Status);
static void_t time_Init(void_t);
static void_t delay_Ms(uint32_t ms);
static void_t appInitCommon(void_t);
static void_t ledBlink(void_t);

/******************************************************************************/

int main(void_t)
{
	appInitCommon();
	while(1)
	{
		ledBlink();
	}
}

/*
 * @func   Blink Led
 * @brief  ledBlink
 * @param  None
 * @retval None
 */
static void_t ledBlink(void_t)
{
	ledControl_SetState(LED_GREEN_PORT,LED_GREEN,GPIO_PIN_SET);
	delay_Ms(1000);
	ledControl_SetState(LED_GREEN_PORT,LED_GREEN,GPIO_PIN_RESET);
	delay_Ms(1000);
}

/*
 * @func   appInitCommon
 * @brief  Initialize peripheral
 * @param  None
 * @retval None
 */
static void_t appInitCommon(void_t)
{
	SystemCoreClockUpdate();
	led1Control_Init();
	time_Init();
}

/*
 * @func   led1Control_Init
 * @brief  Initialize Application
 * @param  None
 * @retval None
 */
static void_t led1Control_Init(void_t)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(LED_GREEN_PORT,&GPIO_InitStructure);
}

/*
 * @func   Set Status Led
 * @brief  ledControl_SetState
 * @param  None
 * @retval None
 */
static void_t ledControl_SetState(GPIO_TypeDef *GPIOx,uint8_t GPIO_PIN,uint8_t Status)
{
	if(Status == GPIO_PIN_SET)
	{
		GPIOx ->BSRRL |= 1 <<GPIO_PIN;
	}
	else
	{
		GPIOx ->BSRRH |= 1 << GPIO_PIN;
	}
}

/*
 * @func   Initialize timer
 * @brief  time_Init
 * @param  None
 * @retval None
 */
static void_t time_Init(void_t)
{
	TIM_TimeBaseInitTypeDef Timer_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	Timer_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	Timer_InitStructure.TIM_Prescaler = TIM_PRESCACLE;
	Timer_InitStructure.TIM_Period = TIM_PERIOD;
	Timer_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer_InitStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1,&Timer_InitStructure);
	TIM_Cmd(TIM1,ENABLE);
}

/*
 * @func   Initialize Delay
 * @brief  Delay Function
 * @param  None
 * @retval None
 */
static void_t delay_Ms(uint32_t ms)
{
	while( ms!=0 )
	{
		TIM_SetCounter(TIM1,0);
		while(TIM_GetCounter(TIM1) != DELAY_MS_1S){};
		ms--;
	}
}
