#ifndef Power_detection_H
#define Power_detection_H


/* Power_detection������ */
portTASK_FUNCTION( vPowerTask, pvParameters );
extern uint8_t flag;
extern uint16_t adcResult;

#endif