/*
 * send.h
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */

#ifndef APP_SEND_SEND_H_
#define APP_SEND_SEND_H_

#define MAX_DATA_COMMAND_SIZE					50
#define SOURCE_ENDPOINT_PRIMARY					1
#define DESTINATTION_ENDPOINT					1
#define HC_NETWORK_ADDRESS						0x0000


void SEND_OnOffStateReport(uint8_t endpoint, uint8_t value);
void SEND_ReportInfoHc(void);
void SEND_PIRStateReport(uint8_t endpoint, uint8_t value);
void SendZigDevRequest(void);
#endif /* APP_SEND_SEND_H_ */
