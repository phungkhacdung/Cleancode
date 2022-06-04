/*
 * led.c
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */
#include "app/framework/include/af.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "led.h"

#define TIMER_DIM	TIMER3

uint32_t tickPerUs;

ledStruct ledRGBArray[LED_RGB_COUNT][LED_RGB_ELEMENT] = {LED_RGB_1, LED_RGB_2};
EmberEventControl led1ToggleEventControl, led2ToggleEventControl;
EmberEventControl* ledEventControl[LED_RGB_COUNT];
ledActionInfo_t ledAction[LED_RGB_COUNT];

void ledInit(void)
{
//	TIMER_Init_TypeDef timerInit = TIMER_DIM_INIT;

	CMU_ClockEnable(cmuClock_GPIO, 1);
//	for(int i = 0; i < LED_RGB_COUNT; i++)
//	{
//		for(int j = 0; j < LED_RGB_ELEMENT; i++)//vai~~ i++ @@
//		{
//			GPIO_PinModeSet(ledRGBArray[i][j].port, ledRGBArray[i][j].pin, gpioModePushPull, 0);
//		}
//	}
	for(int i=0; i<LED_RGB_COUNT; i++)
	{
	  for(int j=0; j<LED_RGB_ELEMENT; j++)
	  {
		  GPIO_PinModeSet(ledRGBArray[i][j].port,
						  ledRGBArray[i][j].pin,
						  gpioModePushPull,
						  0);
	  }
	}

	//Init timer for PWM
//	TIMER_Init_TypeDef timerInit = TIMER_DIM_INIT;
//	CMU_ClockEnable(cmuClock_TIMER3, true);
//
//	TIMER_Init(TIMER3, &timerInit);
//	TIMER_IntEnable(TIMER3, _TIMER_IF_OF_MASK);
//	NVIC_EnableIRQ(TIMER3_IRQn);
//
//	TIMER3->CMD = TIMER_CMD_STOP;
//
//	tickPerUs=(CMU_ClockFreqGet(cmuClock_TIMER3)/(timerInit.prescale+1))/1000000 +1;

	ledEventControl[LED_1] = (EmberEventControl*)&led1ToggleEventControl;
	ledEventControl[LED_2] = (EmberEventControl*)&led2ToggleEventControl;

	//Initialize for all LED(turn off all)
	offLed(LED_1);
	offLed(LED_2);
}

void onLed(ledNumber ledIndex, ledColor color)
{
	for(int i = 0; i < LED_RGB_ELEMENT; i++)
	{
		if(((color >> i) & 0x01) == 1)
		{
			GPIO_PinOutClear(ledRGBArray[ledIndex][i].port, ledRGBArray[ledIndex][i].pin);
		}
		else
		{
			GPIO_PinOutSet(ledRGBArray[ledIndex][i].port, ledRGBArray[ledIndex][i].pin);
		}
	}
}

void offLed(ledNumber ledIndex)
{
	for(int i = 0; i < LED_RGB_ELEMENT; i++)
	{
		GPIO_PinOutSet(ledRGBArray[ledIndex][i].port, ledRGBArray[ledIndex][i].pin);
	}
}

void toggleLed(ledNumber ledIndex, ledColor color, uint8_t toggleTime, uint32_t ontimeMs, uint32_t offtimeMs)
{
	ledAction[ledIndex].ledBlinkMode = LED_TOGGLE;
	ledAction[ledIndex].color = color;
	ledAction[ledIndex].ledAct.ledBlink.onTime = ontimeMs;
	ledAction[ledIndex].ledAct.ledBlink.offTime = offtimeMs;
	ledAction[ledIndex].ledAct.ledBlink.blinkTime = toggleTime*2;

	emberEventControlSetInactive(*ledEventControl[ledIndex]);
	emberEventControlSetActive(*ledEventControl[ledIndex]);
}
void toggleLedHandle(ledNumber ledIndex)
{
	if(ledAction[ledIndex].ledAct.ledBlink.blinkTime != 0)
	{
		if((ledAction[ledIndex].ledAct.ledBlink.blinkTime % 2) != 0 )
		{
			for(int i = 0; i < LED_RGB_ELEMENT; i++)
			{
				if(((ledAction[ledIndex].color >> i) & 0x01) == 1)
				{
					GPIO_PinOutClear(ledRGBArray[ledIndex][i].port, ledRGBArray[ledIndex][i].pin);
				}
				else
				{
					GPIO_PinOutSet(ledRGBArray[ledIndex][i].port, ledRGBArray[ledIndex][i].pin);
				}
			}
			emberAfEventControlSetDelayMS(ledEventControl[ledIndex], ledAction[ledIndex].ledAct.ledBlink.onTime);
		}
		else
		{
			for(int j = 0; j < LED_RGB_ELEMENT; j++)
			{
				GPIO_PinOutSet(ledRGBArray[ledIndex][j].port, ledRGBArray[ledIndex][j].pin);
			}
			emberAfEventControlSetDelayMS(ledEventControl[ledIndex], ledAction[ledIndex].ledAct.ledBlink.offTime);
		}
		ledAction[ledIndex].ledAct.ledBlink.blinkTime--;
	}
	else
	{
		ledAction[ledIndex].ledBlinkMode = LED_FREE;
		for(int j = 0; j < LED_RGB_ELEMENT; j++)
		{
			GPIO_PinOutSet(ledRGBArray[ledIndex][j].port, ledRGBArray[ledIndex][j].pin);
		}
	}
}
void led1ToggleEventHandler(void)
{
	emberEventControlSetInactive(*ledEventControl[LED_1]);
	toggleLedHandle(LED_1);
}
void led2ToggleEventHandler(void)
{
	emberEventControlSetInactive(*ledEventControl[LED_2]);
	toggleLedHandle(LED_2);
}
void InitDimLed(void)
{
	TIMER_Init_TypeDef timerInit = TIMER_DIM_INIT;

	CMU_ClockEnable(cmuClock_TIMER3, true);

	timerInit.prescale = (TIMER_Prescale_TypeDef)_TIMER_CFG_PRESC_DIV1;

	TIMER_Init(TIMER3, &timerInit);

	TIMER_IntEnable(TIMER3, _TIMER_IF_OF_MASK);
	NVIC_EnableIRQ(TIMER3_IRQn);

	TIMER3->CMD = TIMER_CMD_STOP;

	tickPerUs = (CMU_ClockFreqGet(cmuClock_TIMER3)/(timerInit.prescale+1))/1000000 + 1;
}

void LED_SetChangeDimStateTime(uint32_t time)
{
//	emberAfCorePrintln("LED_SetChangeDimStateTime %d\n", time);
	TIMER_CounterSet(TIMER3, 0);
	TIMER_TopSet(TIMER3, time*tickPerUs);

	TIMER3->CMD = TIMER_CMD_START;
}

void TIMER3_IRQHandler(void)
{
	TIMER3->CMD = TIMER_CMD_STOP;

	for(uint8_t i = 0; i < LED_RGB_COUNT; i++)
	{
		if(ledAction[i].ledBlinkMode == LED_DIM)
		{
			dimLedHandle(i);
			break;
		}
	}

	TIMER_IntClear(TIMER3, _TIMER_IF_OF_MASK);
}

void dimLedHandle(ledNumber ledIndex)
{
	uint32_t cycles;

	if(ledAction[ledIndex].ledAct.ledDim.dimHigh)
	{
		for(int i = 0; i < LED_RGB_ELEMENT; i++)
		{
			if(((ledAction[ledIndex].color >> i) & 0x01)== 1)
			{
				GPIO_PinOutClear(ledRGBArray[ledIndex][i].port, ledRGBArray[ledIndex][i].pin);
			}
			else
			{
				GPIO_PinOutSet(ledRGBArray[ledIndex][i].port, ledRGBArray[ledIndex][i].pin);
			}
		}
		ledAction[ledIndex].ledAct.ledDim.dimHigh = false;
		cycles = ledAction[ledIndex].ledAct.ledDim.dimLevel;
	}
	else
	{
		for(int j = 0; j <LED_RGB_ELEMENT; j++)
		{
			GPIO_PinOutSet(ledRGBArray[ledIndex][j].port, ledRGBArray[ledIndex][j].pin);
		}
		ledAction[ledIndex].ledAct.ledDim.dimHigh = true;
		cycles = LED_RGB_DIM_T - ledAction[ledIndex].ledAct.ledDim.dimLevel;
	}
	LED_SetChangeDimStateTime(cycles);
}

void dimLed(ledNumber ledIndex, ledColor color, uint8_t dimLevel)
{
	LED_TimeReset();

	if(dimLevel > 100)
	{
		dimLevel = 100;
	}

	ledAction[ledIndex].ledBlinkMode = LED_DIM;
	ledAction[ledIndex].color = color;
	ledAction[ledIndex].ledAct.ledDim.dimLevel = dimLevel;
	ledAction[ledIndex].ledAct.ledDim.dimHigh = false;

	LED_SetChangeDimStateTime(LED_RGB_DIM_T - dimLevel);
}

void LED_TimeReset(void)
{
TIMER_CounterSet(TIMER3, 0);
TIMER3->CMD = TIMER_CMD_STOP;
}


