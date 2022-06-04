/*
 * led.h
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */
#include "stddef.h"
#include "em_cmu.h"
#include "em_gpio.h"


#ifndef MID_LED_LED_H_
#define MID_LED_LED_H_




#define LED_RGB_COUNT			2
#define LED_RGB_ELEMENT			3
#define LED_TOGGLE_TIME			500

#define LED_PORT_1				gpioPortA
#define LED_BLUE_PIN_1			(0U)
#define LED_GREEN_PIN_1			(3U)
#define LED_RED_PIN_1			(4U)

#define LED_PORT_2				gpioPortD
#define LED_BLUE_PIN_2			(2U)
#define LED_GREEN_PIN_2			(1U)
#define LED_RED_PIN_2			(0U)

//#define LED_RGB_1				{{ {{LED_PORT_1,LED_BLUE_PIN_1}} , {{LED_PORT_1,LED_GREEN_PIN_1}} , {{LED_PORT_1,LED_RED_PIN_1}} }}
//#define LED_RGB_2				{{ {{LED_PORT_2,LED_BLUE_PIN_2}} , {{LED_PORT_2,LED_GREEN_PIN_2}} , {{LED_PORT_2,LED_RED_PIN_2}} }}

#define LED_RGB_1				{ {LED_PORT_1,LED_RED_PIN_1} , {LED_PORT_1,LED_GREEN_PIN_1} , {LED_PORT_1,LED_BLUE_PIN_1} }
#define LED_RGB_2				{ {LED_PORT_2,LED_RED_PIN_2} , {LED_PORT_2,LED_GREEN_PIN_2} , {LED_PORT_2,LED_BLUE_PIN_2} }


#define LED_RGB_DIM_T			100

#define MAX_U32					0xFFFFFFFF

#define CLOCK_PER_1US

#define TIMER_DIM_INIT                                                               \
{                                                                                     \
    false,                /* Enable timer when init complete. */                      \
    false,                /* Stop counter during debug halt. */                       \
    timerPrescale2,      /* No prescaling. */                                        \
    timerClkSelHFPerClk,  /* Select HFPER clock. */                                   \
    false,                /* Not 2x count mode. */                                    \
    false,                /* No ATI. */                                               \
    timerInputActionNone, /* No action on falling input edge. */                      \
    timerInputActionNone, /* No action on rising input edge. */                       \
    timerModeUp,          /* Up-counting. */                                          \
    false,                /* Do not clear DMA requests when DMA channel is active. */ \
    false,                /* Select X2 quadrature decode mode (if used). */           \
    false,                /* Disable one shot. */                                     \
    false                 /* Not started/stopped/reloaded by other timers. */         \
}

typedef enum
{
	ledOff		= 0x000,
	ledRed		= BIT(0),
	ledGreen	= BIT(1),
	ledBlue	    = BIT(2),
	ledPink	    = ledRed | ledBlue, // ledBlue + ledRed
	ledYellow	= ledGreen | ledRed,  // ledGreen +ledRed
	ledRGB		= ledRed | ledGreen | ledBlue
}ledColor;
 // mach ve sai chan GREEN va RED
typedef enum
{
	LED_1,
	LED_2
}ledNumber;

enum
{
	LED_OFF,
	LED_ON
};

enum
{
	LED_TOGGLE,
	LED_DIM,
	LED_FREE,
};

typedef struct
{
	 GPIO_Port_TypeDef   port;
	 unsigned int        pin;
}ledStruct;

typedef struct
{
	uint16_t onTime;
	uint16_t offTime;
	uint8_t  blinkTime;
}ledBlink_t;

typedef struct
{
	boolean dimHigh;
	uint8_t dimLevel;
}ledDim_t;


typedef struct
{
	uint8_t   ledBlinkMode;
	ledColor  color;

	union
	{
		ledBlink_t ledBlink;
		ledDim_t   ledDim;
	}ledAct;

}ledActionInfo_t;

void InitDimLed(void);
void LED_SetChangeDimStateTime(uint32_t time);
void dimLedHandle(ledNumber ledIndex);
void LED_TimeReset(void);
void ledInit(void);
void toggleLed(ledNumber  ledIndex, ledColor color, uint8_t toggleTime,uint32_t onTimeMs,uint32_t offTimeMs);
void dimLed(ledNumber ledIndex, ledColor color,uint8_t dimLevel);
void offLed(ledNumber ledIndex);
void onLed(ledNumber ledIndex, ledColor color);


#endif /* MID_LED_LED_H_ */
