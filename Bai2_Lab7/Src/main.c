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

#include "stm32f401re_rcc.h"
#include "stm32f401re_gpio.h"
#include "stm32f401re_tim.h"
#include "stm32f401re_usart.h"
#include "misc.h"
#include "typedefs.h"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define USART2_TX					GPIO_Pin_2
#define TIME_UPDATE                  (TIM2->CNT)
#define TIME_LIMIT_SEND_DATA           2000     //1s


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

u32_t dwNumber_Press, dwStatus;
u32_t dwTime_firstPress, dwTime_LastPress;
u32_t dwTime_Rising;

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
static void_t AppInitCommon(void_t);
static void_t TIM2_Configuration(void_t);
static void_t USART2_Init(void_t);
static void_t Send_NumberPress(void_t);
static void_t Check_Tim_Press(void_t);

/******************************************************************************/


/* Private macro -------------------------------------------------------------*/


/* Private function ----------------------------------------------------------*/

/* Private Value--------------------------------------------------------------*/


int main(void_t)
{
	AppInitCommon();

	while(1)
	{
		Send_NumberPress();
	}
}

/**
 * @func   Initializes
 * @brief  Initializes All Periperal
 * @param  None
 * @retval None
 */
static void_t
AppInitCommon(void_t)
{
	// Use clock is 84MHz------------------------------------------------------
	SystemCoreClockUpdate();

	// Initializes system tick-------------------------------------------------
	TIM2_Configuration();

	// Initializes USART-------------------------------------------------------
	USART2_Init();
}

/**
 * @func   Check Number Press And Tim Press
 * @brief  Check_Tim_Press
 * @param  None
 * @retval None
 */
static
void_t Check_Tim_Press(void_t)
{
	static uint8_t dwStatus1 = 0;

	dwStatus1 = !dwStatus1;

	if(dwStatus1 == 1)
	{
		dwNumber_Press++;
	}
	else if (dwStatus1 == 0)
	{
		dwTime_Rising = TIM_GetCapture2(TIM2);
		dwStatus = 1;
	}
}

/**
 * @func   Send Number Press To PC
 * @brief  Send_NumberPress
 * @param  None
 * @retval None
 */
static
void_t Send_NumberPress(void_t)
{
	u32_t Tim_SendData = 0;

	if (dwStatus == 1)
    {
    	if (TIME_UPDATE < dwTime_Rising)
    	{
    		Tim_SendData = (0xffff + TIME_UPDATE) - dwTime_Rising;
    	}

    	else
    	{
    		Tim_SendData = TIME_UPDATE - dwTime_Rising;
    	}

    	if (Tim_SendData > TIME_LIMIT_SEND_DATA)
    	{
    		USART_SendData(USART2, dwNumber_Press);
    		dwStatus = 0;
    		dwNumber_Press = 0;
    	}
    }
}

/**
 * @func   Use USART2
 * @brief  Initializes USART2
 * @param  None
 * @retval None
 */
static
void_t USART2_Init(void_t)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    //Enable GPIO clock --------------------------------------------------------
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

    //Configure USART Tx as alternate function push-pull------------------------
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    //Connect USART pins--------------------------------------------------------
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

    GPIO_InitStructure.GPIO_Pin = USART2_TX;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Enable USART clock--------------------------------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);

	//Enable USART--------------------------------------------------------------
    USART_Cmd(USART2, ENABLE);
}

/**
 * @func   Initializes
 * @brief  Initializes All Periperal
 * @param  None
 * @retval None
 */
static
void_t TIM2_Configuration(void_t)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_ICInitTypeDef         TIM_ICInitStructure;
	NVIC_InitTypeDef          NVIC_InitStructure;

	//GPIO clock enable---------------------------------------------------------
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//TIM2 clock enable---------------------------------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 41999;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//Configure Input Capture-----------------------------------------------
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;

	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	TIM_Cmd(TIM2, ENABLE);

	//Configure Input Capture Interrupt--------------------------------------
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @func   Function Interrupt
 * @brief  TIM2_IRQHandler
 * @param  None
 * @retval None
 */
void_t TIM2_IRQHandler(void_t)
{
   if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
   {
	   Check_Tim_Press();
   }

   TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
}

/* END_FILE */
