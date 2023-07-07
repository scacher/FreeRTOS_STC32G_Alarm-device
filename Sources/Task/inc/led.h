#ifndef LED_H
#define LED_H

portTASK_FUNCTION( vLEDTask, pvParameters );
void ledA1hz(void);
void ledB4C4hz(void);
void ledB4hz(void);
void ledC4hz(void);

#endif