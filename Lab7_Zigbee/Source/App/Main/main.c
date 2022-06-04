/*
 * main.c
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */

#include "app/framework/include/af.h"
#include "main.h"
#include "stdio.h"
#include "stdint.h"
#include "Source/Mid/Led/led.h"
#include "Source/App/Network/network.h"
#include "Source/App/Send/send.h"
#include "protocol/zigbee/stack/include/binding-table.h"
#include "Source/Mid/Button/button.h"
#include "Source/Mid/PIR/pir.h"

_Bool networkReady = false;
 systemState system_State;

 EmberEventControl mainStateEventControl;

void Main_ButtonPressCallbackHandler(uint8_t button, BUTTON_Event_t pressHandler);
void Main_ButtonHoldCallbackHandler(uint8_t button, BUTTON_Event_t pressHandler);
void USER_networkEventHandler(uint8_t networkResult);
void Main_PIREventHandler(uint8_t pirAction);

void emberAfMainInitCallback(void)
{
	system_State = POWER_ON_STATE;
	emberAfCorePrintln("Main Init");
	ledInit();
	Network_Init(USER_networkEventHandler);
	emberEventControlSetActive(mainStateEventControl);
	buttonInit(Main_ButtonHoldCallbackHandler, Main_ButtonPressCallbackHandler);
	if(networkReady == true)
	{
		PIR_Init(Main_PIREventHandler);
	}
}

void Main_PIREventHandler(uint8_t pirAction)
{
	static boolean sendFlag = true;
	switch (pirAction) {
		case PIR_MOTION:
			if(sendFlag)
			{
				sendFlag = false;
				SEND_PIRStateReport(2,PIR_MOTION);
			}
			toggleLed(LED_1,ledPink,1,150,150);
			break;
		case PIR_UNMOTION:
			sendFlag = true;
			toggleLed(LED_1,ledRed,1,150,150);
			emberAfCorePrintln("PIR_UNMOTION");
			SEND_PIRStateReport(2,PIR_UNMOTION);
			break;

		default:
			break;
	}
}

void mainStateEventHandler(void)
{
	emberEventControlSetInactive(mainStateEventControl);

	EmberNetworkStatus nwkStatusCurrent;

	switch (system_State) {
		case POWER_ON_STATE:
			nwkStatusCurrent = emberAfNetworkState();
			if(nwkStatusCurrent == EMBER_NO_NETWORK)
			{
				toggleLed(LED_1,ledRed,3,300,300);
				NETWORK_FindAndJoin();
			}
			system_State = IDLE_STATE;
			break;
		case REPORT_STATE:
			toggleLed(LED_2,ledGreen,3,300,300);
			networkReady = true;
			system_State = IDLE_STATE;
			SEND_ReportInfoHc();
			break;
		case IDLE_STATE:
			break;
		case REBOOT_STATE:
			system_State = IDLE_STATE;
		//	halReboot();
			break;
		default:
			break;
	}

}

void USER_networkEventHandler(uint8_t networkResult)
{
	emberAfCorePrintln("Network Event Handle");
	switch (networkResult) {
		case NETWORK_HAS_PARENT:
			emberAfCorePrintln("Network has parent");
			networkReady = true;
			system_State = REPORT_STATE;
			emberEventControlSetDelayMS(mainStateEventControl, 1000);
			break;
		case NETWORK_JOIN_FAIL:
			system_State = POWER_ON_STATE;
			emberAfCorePrintln("Network Join Fail");
			emberEventControlSetDelayMS(mainStateEventControl, 1000);
			break;
		case NETWORK_JOIN_SUCCESS:
			emberAfCorePrintln("Network Join Success");
			networkReady =true;
			system_State = REPORT_STATE;
			emberEventControlSetDelayMS(mainStateEventControl, 1000);
			break;
		case NETWORK_LOST_PARENT:
			emberAfCorePrintln("Network lost parent");
			toggleLed(LED_1,ledPink,3,300,300);
			system_State = IDLE_STATE;
			emberEventControlSetDelayMS(mainStateEventControl, 1000);
			break;
		case NETWORK_OUT_NETWORK:
			if(networkReady)
			{
				emberAfCorePrintln("Network Out network");
				toggleLed(LED_1,ledPink,3,300,300);
				system_State = REBOOT_STATE;
				emberEventControlSetDelayMS(mainStateEventControl, 3000);
			}
			break;
		default:
			break;
	}
}





void Main_ButtonPressCallbackHandler(uint8_t button, BUTTON_Event_t pressHandler)
{
//	if((pressHandler > press_max) && (pressHandler <hold_max))
//	{
//		emberAfCorePrintln("SW %d HOLDING %d s\n",button+1,pressHandler-press_max);
//	} else if((pressHandler > 0) && (pressHandler <press_max))
//	{
//		emberAfCorePrintln("SW %d PRESS_TIME %d s\n",button+1,pressHandler-press_max);
//	}else
//	{
//		emberAfCorePrintln("UNKNOW\n");
//	}
	switch(pressHandler)
	{
	case press_1:
		if(button == SW_1)
		{
			emberAfCorePrintln("Version 1");
			emberAfCorePrintln("SW1: 1 time");
			onLed(LED_2,ledGreen);
			SEND_OnOffStateReport(1, LED_ON);

		}
	break;
	case press_2:
		if(button == SW_1)
		{
			emberAfCorePrintln("Version 1");
			emberAfCorePrintln("SW1: 2 times");
			SEND_OnOffStateReport(1, LED_OFF);
			offLed(LED_2);
		}

		break;
	case press_5:
		if(button == SW_1)
		{
			emberAfCorePrintln("SW1: 5 time");
			toggleLed(LED_2,ledYellow, 2, 200, 200);
			emberLeaveNetwork();
			//halReboot();

		}
		break;
	default:
		break;
	}
}

void Main_ButtonHoldCallbackHandler(uint8_t button, BUTTON_Event_t holdingHandler)
{
//	emberAfCorePrintln("SW %d HOLDING %d s\n",button+1,holdingHandler-press_max);
	switch(holdingHandler)
	{
	case hold_1s:
		emberAfCorePrintln("SW1: 1 s");
		break;
	case hold_2s:
		emberAfCorePrintln("SW1: 2 s");
		break;
	case hold_3s:
		emberAfCorePrintln("SW1: 3 s");
		break;
	case hold_4s:
		emberAfCorePrintln("SW1: 4 s");
		break;
	case hold_5s:
		emberAfCorePrintln("SW1: 5 s");
		break;

	default:
		break;
	}
}
