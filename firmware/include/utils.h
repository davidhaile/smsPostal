// utils.h

#define LED_OFF LOW
#define LED_ON  HIGH

void LED(bool);
void LED_toggle();
void LED_on();
void LED_off();
void displayTime();
uint16_t byteSwap(uint16_t);

extern int led;
extern FuelGauge fuel;
