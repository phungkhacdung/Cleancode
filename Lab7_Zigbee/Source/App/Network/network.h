/*
 * network.h
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */

#ifndef APP_NETWORK_NETWORK_H_
#define APP_NETWORK_NETWORK_H_

typedef enum{
	NETWORK_HAS_PARENT,
	NETWORK_JOIN_FAIL,
	NETWORK_JOIN_SUCCESS,
	NETWORK_LOST_PARENT,
	NETWORK_OUT_NETWORK,
}Network_Event_e;

typedef void (*networkEventHandler) (Network_Event_e networkEvent);

void NETWORK_FindAndJoin(void);
void joinNetworkEventHandler(void);
void Network_Init(networkEventHandler networkResult);
#endif /* APP_NETWORK_NETWORK_H_ */
