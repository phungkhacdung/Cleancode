/*
 * network.c
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */
#include "app/framework/include/af.h"
#include "network.h"

uint32_t timeFindAndJoin = 0;

networkEventHandler networkEventHandle = NULL;

EmberEventControl joinNetworkEventControl;

void Network_Init(networkEventHandler networkResult)
{
	networkEventHandle = networkResult;
}
void NETWORK_FindAndJoin(void)
{
	if(emberAfNetworkState() == EMBER_NO_NETWORK)
	{
		emberEventControlSetDelayMS(joinNetworkEventControl, 2000);
	}
}

void joinNetworkEventHandler(void)
{
	emberEventControlSetInactive(joinNetworkEventControl);

	if(emberAfNetworkState() == EMBER_NO_NETWORK)
	{
		emberAfPluginNetworkSteeringStart();
		timeFindAndJoin++;
		emberEventControlSetDelayMS(joinNetworkEventControl, 2000);
	}
}

void NETWORK_StopFindAndJoin(void)
{
	emberAfPluginNetworkSteeringStop();
}

bool emberAfStackStatusCallback(EmberStatus status)
{

	emberAfCorePrintln("emberAfStackStatusCallback\n");
	emberAfCorePrintln("%x",status);

		if(status == EMBER_NETWORK_UP)
		{
			if(timeFindAndJoin>0)// vao mang thanh cong
			{
				NETWORK_StopFindAndJoin();
				if(networkEventHandle != NULL)
				{
					networkEventHandle(NETWORK_JOIN_SUCCESS);
					emberAfCorePrintln("NETWORK_JOIN_SUCCESS");
				}
			}else
			{
				if(networkEventHandle != NULL)
				{
					networkEventHandle(NETWORK_HAS_PARENT);
					emberAfCorePrintln("NETWORK_HAS_PARENT");
				}
			}

		}
		else
			{
				EmberNetworkStatus nwkStatusCurrent = emberAfNetworkState();
				if(nwkStatusCurrent == EMBER_NO_NETWORK)
				{
					if(networkEventHandle != NULL)
					{
						networkEventHandle(NETWORK_OUT_NETWORK);
						emberAfCorePrintln("NETWORK_OUT_NETWORK");
					}
				}
				else if(nwkStatusCurrent == EMBER_JOINED_NETWORK_NO_PARENT){
					emberAfCorePrintln("NETWORK_LOST_PARENT");
					networkEventHandle(NETWORK_LOST_PARENT);
				}
			}

	return false;
}

