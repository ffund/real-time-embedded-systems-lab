#ifndef RGBLED_H
#define RGBLED_H

/*  Sets up three PWM channels for controlling RGB LED */
void initPwm();

/* Set color of RGB LED by changing duty cycle of PWM outputs */
void setColor(uint8_t, uint8_t, uint8_t);

#endif
