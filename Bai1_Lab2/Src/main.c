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
 * Last Changed:     $Date: $May 31, 2022
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include <stdint.h>
#include "stm32f401re_gpio.h"
#include "stm32f401re_rcc.h"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
enum {
	BUTTON_PRESS = 0 ,
	BUTTON_RELEASE = 1
};

//define logic GPIO_PIN
#define GPIO_PIN_SET		1
#define GPIO_PIN_RESET		0
#define GPIO_PIN_LOW		0
#define GPIO_PIN_HIGH		1

//define GPIO_PIN
#define LED_GPIO_PORT			GPIOA
#define LED_GPIO_PIN			GPIO_Pin_5
#define LED_PIN_5				5
#define LEDCONTROL_SETCLOCK		RCC_AHB1Periph_GPIOA

#define BUTTON_GPIO_PORT		GPIOC
#define BUTTON_GPIO_PIN			GPIO_Pin_13
#define BUTTON_PIN_13			13
#define BUTTONCONTROL_SETCLOCK	RCC_AHB1Periph_GPIOC

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

void_t delay_Ms(void_t);
static void_t led_Init(void_t);
static void_t ledControl_SetStatus(void_t);
static void_t button_Init(void_t);
static void_t ledControl_SetStatus(GPIO_TypeDef *GPIOx,uint8_t GPIO_PIN,uint8_t Status);
static uint8_t buttonRead_Status(GPIO_TypeDef *GPIOx,uint32_t GPIO_PIN);


/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/

int main(void_t)
{
	led_Init();
	button_Init();
	bool_t boCheckStatus = 0;
	while(1)
	{

		if(ButtonRead_Status(BUTTON_GPIO_PORT,BUTTON_PIN_13) == BUTTON_PRESS)
		{
			delay_Ms(20);
			if(ButtonRead_Status(BUTTON_GPIO_PORT,BUTTON_PIN_13) == BUTTON_PRESS)
			{
				boCheckStatus = !boCheckStatus;
				ledControl_SetStatus(LED_GPIO_PORT, LED_PIN_5,boCheckStatus);
				while(buttonRead_Status(BUTTON_GPIO_PORT,BUTTON_PIN_13) == BUTTON_PRESS);
			}
		}
	}
}

/*
 * @func   delay_Ms
 * @brief  Delay funtion
 * @param  None
 * @retval None
 */
void_t delay_Ms(uint32_t milisecond)
{
	for(uint32_t i= 0;i<milisecond;i++)
	{
		for(uint32_t j=0;j<5000;j++);
	}
}

/*
 * @func   led_Init
 * @brief  Initialize led
 * @param  None
 * @retval None
 */
static
void_t led_Init(void_t)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LEDCONTROL_SETCLOCK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(LED_GPIO_PORT,&GPIO_InitStructure);
}

/*
 * @func   button_Init
 * @brief  Initialize button
 * @param  None
 * @retval None
 */
static
void_t button_Init(void_t)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(BUTTONCONTROL_SETCLOCK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = BUTTON_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(BUTTON_GPIO_PORT,&GPIO_InitStructure);
}

/*
 * @func   ledControl_SetStatus
 * @brief  Control Status Led
 * @param  GPIO_TypeDef, GPIO_PIN, Status
 * @retval None
 */
static
void_t ledControl_SetStatus(GPIO_TypeDef *GPIOx,uint8_t GPIO_PIN,uint8_t Status)
{
	if(Status == GPIO_PIN_SET)
	{
		GPIOx->BSRRL |= 1 << GPIO_PIN;
	}
	if(Status == GPIO_PIN_RESET)
	{
		GPIOx->BSRRH |= 1 << GPIO_PIN;
	}
}

/*
 * @func   buttonRead_Status
 * @brief  Control Status Button
 * @param  GPIO_TypeDef, GPIO_PIN
 * @retval Button_Pin
 */
static
uint8_t buttonRead_Status(GPIO_TypeDef *GPIOx,uint32_t GPIO_PIN)
{
	uint32_t Read_Pin;
	Read_Pin = (GPIOx->IDR) >> GPIO_PIN;
	Read_Pin = Read_Pin & 0x01;
	return Read_Pin;
}
