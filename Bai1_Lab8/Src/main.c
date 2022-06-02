/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
*******************************************************************************
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
#include "stm32f401re_adc.h"
#include "stm32f401re_usart.h"
#include "timer.h"
#include "typedefs.h"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/

#define USART2_TX					GPIO_Pin_2

#define ADC_GPIO_PORT				GPIOA
#define ADC_GPIO_PIN				GPIO_Pin_0
#define ADCx_SENSOR					ADC1
#define	ADCx_CLK					RCC_APB2Periph_ADC1

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
static u32_t dwTime_Current,dwTime_Initial;
static u32_t dwTime_Total;

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

static void delayMs(u32_t milisecond);
static void ADC1_Init(void);
static void Cover_Temperature(void);
static void USART2_Init(void);
static void multiSensorScan(void);

/******************************************************************************/

int main(void)
{
	SystemCoreClockUpdate();
	TimerInit();
	ADC1_Init();
	USART2_Init();
	dwTime_Initial = GetMilSecTick();
	ADC_SoftwareStartConv(ADC1);
	while(1)
	{
		multiSensorScan();
	}
}
static void ADC1_Init(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	ADC_InitTypeDef			ADC_InitStructure;
	ADC_CommonInitTypeDef	ADC_CommnonInitStructure;

	//
	RCC_APB2PeriphClockCmd(ADCx_CLK,ENABLE);


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(ADC_GPIO_PORT,&GPIO_InitStructure);
	//
	ADC_DeInit();
	//
	ADC_CommnonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommnonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommnonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommnonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommnonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;

	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_144Cycles);

	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_Cmd(ADC1,ENABLE);

}

static void Cover_Temperature(void)
{
	float_t TemperatureValueLast,TemperatureValue;

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	TemperatureValue = ADC_GetConversionValue(ADC1);

	TemperatureValue *= 3300;

	TemperatureValue /= 4096;

	TemperatureValue /= (float_t)1000.0;

	TemperatureValue -= (float_t)0.76;

	TemperatureValue = (float_t)0.0025;

	TemperatureValueLast = TemperatureValue + (float_t)25.0;

	USART_SendData(USART2,TemperatureValueLast);
}

static void USART2_Init(void)
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

    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);

	//Enable USART--------------------------------------------------------------
    USART_Cmd(USART2, ENABLE);
}

static
void multiSensorScan(void)
{
	dwTime_Current = GetMilSecTick();
	if(dwTime_Current >= dwTime_Initial)
	{
		dwTime_Total += dwTime_Current - dwTime_Initial;
	}
	else
	{
		dwTime_Total += 0xFFFFFFFFU - dwTime_Current + dwTime_Initial;
	}
	if(dwTime_Total >= 2000)
	{
		dwTime_Total = 0;
		Cover_Temperature();
	}
	dwTime_Initial = dwTime_Current;
}

static void delayMs(u32_t milisecond)
{
	for(u32_t i= 0;i<milisecond;i++)
	{
		for(u32_t j=0;j<5000;j++);
	}
}
