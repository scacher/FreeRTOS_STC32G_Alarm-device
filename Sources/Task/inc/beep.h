#ifndef BEEP_H
#define BEEP_H


/* PWMBºôÎüµÆÈÎÎñº¯Êı */
portTASK_FUNCTION( vPWMA_beepTask, pvParameters );
void beep_1hz(void);
void beep_4hz(void);
void beep_8hz(void);
//void beep_output(void);

#endif
