/*
 * send.c
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */
#include "app/framework/include/af.h"
#include "send.h"

 #define ZDO_MESSAGE_OVERHEAD

static void SEND_SendCommandUnicast(uint8_t source,
									uint8_t destination,
									uint16_t address)
{
	emberAfSetCommandEndpoints(source, destination);
	(void)emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, address);
}
void SEND_FillBufferGlobalCommand(EmberAfClusterId clusterID,
								  EmberAfAttributeId attributeID,
								  uint8_t globalCommand,
								  uint8_t* value,
								  uint8_t length,
								  uint8_t dataType)
{
	uint8_t data[20];
	data[0] = (uint8_t)(attributeID & 0x00FF);
	data[1] = (uint8_t)((attributeID & 0xFF00)>>8);
	data[2] = EMBER_SUCCESS;
	data[3] = (uint8_t)dataType;
	memcpy(&data[4], value, length);

	(void) emberAfFillExternalBuffer((ZCL_GLOBAL_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER | ZCL_DISABLE_DEFAULT_RESPONSE_MASK),
									  clusterID,
									  globalCommand,
									  "b",
									  data,
									  length + 4);
}
void SEND_ReportInfoHc(void)
{
	uint8_t modelID[14] = {13, 'S', 'W', '1','_','P','I','R','1','_','B','A','T','1'};
	uint8_t mannufactureID[5] = {4, 'L', 'u', 'm', 'i'};
	uint8_t version = 1;

	if(emberAfNetworkState() != EMBER_JOINED_NETWORK)
	{
		return;
	}
	SEND_FillBufferGlobalCommand(ZCL_BASIC_CLUSTER_ID,
								ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID,
								ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								modelID,
								13,
								ZCL_CHAR_STRING_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(1,1,HC_NETWORK_ADDRESS);
}

void SEND_OnOffStateReport(uint8_t endpoint, uint8_t value){
	SEND_FillBufferGlobalCommand(ZCL_ON_OFF_CLUSTER_ID,
						   ZCL_ON_OFF_ATTRIBUTE_ID,
						   ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
						   (uint8_t*) &value,
						   1,
						   ZCL_BOOLEAN_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(endpoint,
								DESTINATTION_ENDPOINT,
								HC_NETWORK_ADDRESS);

	emberAfWriteServerAttribute(endpoint,
								ZCL_ON_OFF_CLUSTER_ID,
								ZCL_ON_OFF_ATTRIBUTE_ID,
								(uint8_t*) &value,
								ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}
void SEND_PIRStateReport(uint8_t endpoint, uint8_t value){
	SEND_FillBufferGlobalCommand(ZCL_IAS_ZONE_CLUSTER_ID,
								 ZCL_ZONE_STATUS_ATTRIBUTE_ID,
								 ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								 (uint8_t*) &value,
								 1,
						   	   	 ZCL_BOOLEAN_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(endpoint,
								DESTINATTION_ENDPOINT,
								HC_NETWORK_ADDRESS);

	emberAfWriteServerAttribute(endpoint,
								ZCL_IAS_ZONE_CLUSTER_ID,
								ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								(uint8_t*) &value,
								ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}

void SendZigDevRequest(void)
{
	uint8_t contents[ZDO_MESSAGE_OVERHEAD + 1];
	contents[0] = 0x00;

	(void) emberSendZigDevRequest(0x0000, LEAVE_RESPONSE, EMBER_AF_DEFAULT_APS_OPTIONS, contents, sizeof(contents));
}
