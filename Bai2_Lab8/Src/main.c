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
#include "stm32f401re_dma.h"
#include "typedefs.h"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/



/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define USART2_TX			GPIO_Pin_2
#define USART2_Baud			57600

#define ADC_PORT			GPIOC
#define ADC_PIN				GPIO_Pin_5

#define ADCx_SENSOR			ADC1
#define ADCx_CLOCK			RCC_APB2Periph_ADC1
#define DMA_CHANNEL			DMA_Channel_0
#define DMA_STREAMx			DMA2_Stream0
#define ADCx_DR_ADDRESS		((u32_t)0x4001204C)

#define TIME_SEND_LIGHT 	5000

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
u16_t wUhADCConvertedValue = 0;

u16_t wLinghtFromSensor  = 0;

static u32_t dwTimeCurrent,dwTimeInitial;
static u32_t dwTimeTotal;

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
static void_t LightSenSor_Init(void_t);
static u16_t LightSenSor_MeasureuseDMA(void_t);
static void_t USART2_Init(void_t);
static void_t MultiSensorScan(void_t);
static void_t delay_Ms(u32_t milisecond);
static void_t appInitCommon(void_t);

/******************************************************************************/


int main(void_t)
{
	appInitCommon();
	while(1)
	{
		MultiSensorScan();
	}
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
	TimerInit();
	LightSenSor_Init();
	USART2_Init();
}

/*
 * @func   LightSenSor_Init
 * @brief  Initialize LighSensor
 * @param  None
 * @retval None
 */
void_t LightSenSor_Init(void_t)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	ADC_InitTypeDef			ADC_InitStructure;
	ADC_CommonInitTypeDef	ADC_CommnonInitStructure;
	DMA_InitTypeDef			DMA_InitStructure;
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(ADCx_CLOCK,ENABLE);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(ADC_PORT,&GPIO_InitStructure);
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

	ADC_Init(ADCx_SENSOR,&ADC_InitStructure);

	//
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32_t)ADCx_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32_t)&wUhADCConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_STREAMx,&DMA_InitStructure);
	DMA_Cmd(DMA_STREAMx,ENABLE);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_3Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADCx_SENSOR, ENABLE);

	ADC_DMACmd(ADCx_SENSOR, ENABLE);

	ADC_Cmd(ADC1,ENABLE);

	ADC_SoftwareStartConv(ADC1);

}

/*
 * @func   LightSenSor_Init
 * @brief	MeasureuseDMA  LightSenSor
 * @param  None
 * @retval None
 */
static u16_t LightSenSor_MeasureuseDMA(void_t)
{
	return wUhADCConvertedValue;
}

/*
 * @func	USART2_Init
 * @brief	Initialize  USART2
 * @param 	None
 * @retval	None
 */
static void_t USART2_Init(void_t)
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

/*
 * @func	USART_Send Light
 * @brief	MultiSensorScan
 * @param 	None
 * @retval	None
 */
static
void_t MultiSensorScan(void_t)
{

	dwTimeCurrent = GetMilSecTick();
	if(dwTimeCurrent >= dwTimeInitial)
	{
		dwTimeTotal += dwTimeCurrent - dwTimeInitial;
	}
	else
	{
		dwTimeTotal += 0xFFFFFFFFU - dwTimeCurrent + dwTimeInitial;
	}
	if(dwTimeTotal >= TIME_SEND_LIGHT)
	{
		dwTimeTotal = 0;
		wLinghtFromSensor = LightSenSor_MeasureuseDMA();
		u8_t wLinghtFromSensor_HightBit = (uint8_t)(wLinghtFromSensor >>8) ;
		u8_t wLinghtFromSensor_LowBit = (uint8_t)(wLinghtFromSensor & 0xFF);
		USART_SendData(USART2,wLinghtFromSensor_HightBit);
		delay_Ms(5);
		USART_SendData(USART2,wLinghtFromSensor_LowBit);
	}
	dwTimeInitial = dwTimeCurrent;

}

void_t delay_Ms(u32_t milisecond)
{
	for(u32_t i= 0;i<milisecond;i++)
	{
		for(u32_t j=0;j<5000;j++);
	}
}
