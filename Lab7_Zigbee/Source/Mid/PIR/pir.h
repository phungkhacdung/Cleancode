/*
 * pir.h
 *
 *  Created on: May 11, 2022
 *      Author: Dung
 */

#ifndef MID_PIR_PIR_H_
#define MID_PIR_PIR_H_


#define PIR_PORT						gpioPortC
#define PIR_PIN							(4U)

typedef enum{
	PIR_UNMOTION,
	PIR_MOTION,
	PIR_STATE_DEBOUNCE,
	PIR_STATE_WAIT_5S,
	PIR_STATE_WAIT_30S

}pirActionHandler_e;

typedef void(*pirControl) (pirActionHandler_e PirEvent);

void PIR_Init(pirControl PirHandler);
void PIR_Enable(boolean enable);

#endif /* MID_PIR_PIR_H_ */
