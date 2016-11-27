#ifndef __accelerometer
#define __accelerometer
#define ADC_TIMEOUT 0xFFF
int acc_init();
//rawinput
int getX();
int getY();
int getZ();

float getgX();
float getgY();
float getgZ();

int ADdetect(float gx, float gy, float gz);

#endif
