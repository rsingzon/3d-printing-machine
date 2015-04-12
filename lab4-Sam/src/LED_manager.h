#ifndef LED_MANAGER_H
#define LED_MANAGER_H



void startPulse();
void ledsOn(int LED);
void initialize_LEDs();
void control_LEDs(float temperature);
void rotate_clockwise();
void rotate_counterclockwise();
void turn_LEDs_off();
void turn_on_LED();
void changeLeds(float angle, int LED);
#endif