/*
 * receive.c
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */
#include "app/framework/include/af.h"
#include "Source/Mid/Led/led.h"
#include "Source/App/Send/send.h"
#include "receive.h"


boolean emberAfPreCommandReceivedCallback(EmberAfClusterCommand* cmd)
{
	if(cmd->clusterSpecific)
	{
		switch(cmd->apsFrame->clusterId)
		{
			case ZCL_ON_OFF_CLUSTER_ID:
				RECEIVE_HandleOnOffCluster(cmd);
				break;
			case ZCL_LEVEL_CONTROL_CLUSTER_ID:
				RECEIVE_HandleLevelControlCluster(cmd);
				break;
			default:
				break;
		}
	}
	return false;
}

boolean emberAfPreMessageReceivedCallback(EmberAfIncomingMessage* incommingMessage)
{
	if(incommingMessage->apsFrame->clusterId == ACTIVE_ENDPOINTS_RESPONSE)
	{

		return true;
	}
 return false;
}

bool RECEIVE_HandleLevelControlCluster(EmberAfClusterCommand* cmd)
{
	uint8_t commandID = cmd->commandId;
	uint8_t endPoint  = cmd->apsFrame -> destinationEndpoint;
	uint8_t payloadOffset = cmd->payloadStartIndex;		// Gan offset = startindex
	uint8_t level;
	uint16_t transitionTime;
	emberAfCorePrintln("ClusterID: 0x%2X", cmd->apsFrame->clusterId);
/******************************************LEVEL CONTROL LED***************************************************************************/
		switch(commandID)
			{

				case ZCL_MOVE_TO_LEVEL_COMMAND_ID:
					if(cmd->bufLen < payloadOffset + 1u)
					{
						return EMBER_ZCL_STATUS_MALFORMED_COMMAND;
					}
					level = emberAfGetInt8u(cmd->buffer, payloadOffset, cmd->bufLen);		// Lay 1 byte gia tri cua level control
					transitionTime = emberAfGetInt16u(cmd->buffer, payloadOffset+1, cmd->bufLen);
					emberAfCorePrintln(" RECEIVE_HandleLevelControlCluster LEVEL = %d, time: 0x%2X\n", level, transitionTime);

					if(endPoint == 1)
					{
						if(level >=80)
						{
							emberAfCorePrintln("LED GREEN");
							onLed(LED_1, ledGreen);
						}else if(level>=40)
						{
							emberAfCorePrintln("LED RED");
							onLed(LED_1, ledRed);
						}else if(level>0)
						{
							emberAfCorePrintln("LED BLUE");
							onLed(LED_1, ledBlue);
						}else
						{
							emberAfCorePrintln("turn 0ff");
							offLed(LED_1);
						}

					}
					break;
				default:
					break;
				}
		return false;
}


bool RECEIVE_HandleOnOffCluster(EmberAfClusterCommand* cmd)
{
	uint8_t commandID = cmd->commandId;
	uint8_t endpoint = cmd ->apsFrame -> destinationEndpoint;
	/************************************ON-OFF LED******************************************************************************************/
	emberAfCorePrintln("RECEIVE_HandleOnOffCluster SourceEndpoint = %d, commandID = %d\n",endpoint,commandID);
	switch(commandID)
	{
	case ZCL_OFF_COMMAND_ID:
		emberAfCorePrintln("Turn off LED");

		switch (cmd->type) {
			case EMBER_INCOMING_UNICAST:
				emberAfCorePrintln("Unicast");
				offLed(LED_1);
				SEND_OnOffStateReport(endpoint, LED_OFF);
				break;
			case EMBER_INCOMING_MULTICAST:
				emberAfCorePrintln("Multicast");
				offLed(LED_1);
				offLed(LED_2);
				break;
			default:
				break;
		}

		break;
	case ZCL_ON_COMMAND_ID:

		emberAfCorePrintln("Turn on LED");
		switch (cmd->type) {
			case EMBER_INCOMING_UNICAST:
				onLed(LED_1, ledRed);
				SEND_OnOffStateReport(endpoint, LED_ON);
				break;
			case EMBER_INCOMING_MULTICAST:
				emberAfCorePrintln("Multicast");
				onLed(LED_2, ledGreen);
				break;
			default:
				break;
		}
		break;
	default:
		break;
	}
	return false;
}
