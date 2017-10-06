//-------------------------------------------------------------------------------------------------
// Main - SMS Postal
//-------------------------------------------------------------------------------------------------
#define GLOBAL
#include "include/environ.h"
#undef GLOBAL

#ifdef SAVE_POWER
SYSTEM_MODE(MANUAL);	// Cell starts OFF?
#else
// lets keep the radio off until we get a fix, or 2 minutes go by.
SYSTEM_MODE(SEMI_AUTOMATIC);
#endif

#ifndef DISABLE_THREADS
	SYSTEM_THREAD(ENABLED);
#endif

/*ApplicationWatchdog wd(660000, System.reset); //This Watchdog code will reset the processor if the dog is not kicked every 11 mins which gives time for 2 modem reset's.*/

//-------------------------------------------------------------------------------------------------
void setup() {
	pinMode(BLUE_LED, OUTPUT);
	Serial.begin(9600);

	memset(&globalData, 0, sizeof(globalData));

#ifdef SAVE_POWER
	Cellular.off();

	// Bright RED indicates cell is off
	RGB.control(true);
	RGB.color(0xff, 0, 0);
	RGB.brightness(50);
#else
	Cellular.on();
	Cellular.connect();

	// 2017-10-01 I doubt if these can work without using the Particle SIM.
	Particle.connect();
	/*Particle.connected()*/
#endif

	// Wait for cell to come up
	/*while (!Cellular.ready()) {
		LED_toggle();
		delay(100);
	}*/

	// Flush buffer
	while (Serial.available()) {
		Serial.read();
	}

	// Setup for mountain daylight time
	Time.zone(-6);

	globalData.system.ready = true;
}

//--------------------------------------------------------------------------------------------------
void loop() {
	#ifdef DISABLE_THREADS
		commands.update();
		/*eeprom.update();*/
		housekeeping.update();
		sms.update();
	#else
		// This application uses tasks, therefore nothing is accomplished in the main loop().
	#endif
}
