#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#define LIGHT_THRESHOLD_HIGH 4000
#define LIGHT_THRESHOLD_LOW  100


/* Set up analog input and ADC to read values from light sensor */
void initAdc();

/* Change thresholds for light sensor readings */
void setAdcThresholds(uint32_t high, uint32_t low);

/* Get current light reading (digital value) from light sensor */
int getLightReading();

#endif
