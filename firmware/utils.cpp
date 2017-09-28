// utils.cpp
#include "application.h"
#include "include/environ.h"

FuelGauge fuel;

void displayTime() {
	Serial.println(Time.timeStr());
}

//------------------------------------------------------------------------------------------------------
void LED(bool status) {
	digitalWrite(BLUE_LED, status);
}

//------------------------------------------------------------------------------------------------------
void LED_toggle() {
	static bool status = false;
	digitalWrite(BLUE_LED, status);
	status = !status;
}

//------------------------------------------------------------------------------------------------------
void LED_on() {
	digitalWrite(BLUE_LED, LED_ON);
}

//------------------------------------------------------------------------------------------------------
void LED_off() {
	digitalWrite(BLUE_LED, LED_OFF);
}

//------------------------------------------------------------------------------------------------------
uint16_t byteSwap(uint16_t value) {
	uint8_t *p;

	p = (uint8_t *)&value;
	return (BYTESWAP(p));
}
