/*
 * receive.h
 *
 *  Created on: Apr 28, 2022
 *      Author: Dung
 */



#ifndef APP_RECEIVE_RECEIVE_H_
#define APP_RECEIVE_RECEIVE_H_

#define LED_OFF								0
#define LED_ON								1


bool RECEIVE_HandleLevelControlCluster(EmberAfClusterCommand* cmd);
boolean emberAfPreCommandReceivedCallback(EmberAfClusterCommand* cmd);
bool RECEIVE_HandleOnOffCluster(EmberAfClusterCommand* cmd);
#endif /* APP_RECEIVE_RECEIVE_H_ */
