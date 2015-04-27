#ifndef POWER_H
#define POWER_H

/* Sets up User button to wake system from STOP mode */
void initWakeupPin();

/* Put system in sleep mode */
void sleep();

/* Put system in stop mode */
void stop();

#endif
