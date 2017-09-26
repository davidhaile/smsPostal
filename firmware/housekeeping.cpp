// Housekeeping - background tasks
#include "include/environ.h"

#define HOUSEKEEPING_DELAY	100

Thread *housekeepingThread;
Housekeeping housekeeping;
#ifdef USE_PMIC_DIAGNOSTICS
	PMIC pmic;
#endif

void updateLED(uint16_t);
void i2c_scan();

//-------------------------------------------------------------------------------------------------
void Housekeeping::update() {
	static elapsedMillis elapsed_ms;
	static elapsedMillis secondCounter_ms;

	CellularSignal signalInfo = Cellular.RSSI();
	globalData.cell.rssi = signalInfo.rssi;				// -113 to -15dBm signal strength. I believe that -113 is better than -51.
	globalData.cell.signalQuality = signalInfo.qual;	// 0-49, where higher numbers indicate better signal quality
	globalData.cell.isValid = Cellular.ready();

	globalData.system.uptime += elapsed_ms;;
	configurationData.system.totalTime += elapsed_ms;
	elapsed_ms = 0;

	#ifdef USE_PMIC_DIAGNOSTICS
	// 2017-08-24 Can't figure out how to access these functions!
		bool status;
		uint8_t value;
		char buffer[80];
		SINGLE_THREADED_BLOCK() {
			status = pmic.isPowerGood();
			sprintf(buffer, "PG: %d, ", status); Serial.print(buffer);
			/*status = pmic.getVbusStat();
			sprintf(buffer, "VB: %d, ", status); Serial.print(buffer);*/
			value = pmic.readInputSourceRegister();
			sprintf(buffer, "IS: %x, ", value); Serial.print(buffer);
			value = pmic.readPowerONRegister();
			sprintf(buffer, "PO: %x, ", value); Serial.print(buffer);
			Serial.println();
		}
	#endif

	// Ambient light
	int value = analogRead(PHOTO_RESISTOR);
	globalData.system.ambientLight = value/1024.0;

	//------------------------------------------------------------------------------------------------
	// One second tasks
	//------------------------------------------------------------------------------------------------
	if (secondCounter_ms >= 1000) {
		secondCounter_ms = 0;

		if (globalData.cell.countdownTimer > 0) {
			globalData.cell.countdownTimer--;
		}

		globalData.hardware.batteryChargePercent = fuel.getSoC();

	}

	updateLED(HOUSEKEEPING_DELAY);
}

//-------------------------------------------------------------------------------------------------
os_thread_return_t housekeepingTask() {
	WAIT_UNTIL_SYSTEM_IS_READY;

	while (true) {
		/*i2c_scan();*/
		housekeeping.update();
		delay(HOUSEKEEPING_DELAY);
	}
}

//-------------------------------------------------------------------------------------------------
void Housekeeping::open() {

	// Number of seconds on startup that LED indicates cellular strength
	globalData.cell.countdownTimer = 60;

	// To support Grove devices
	Wire.begin();

	// Ambient Light
	pinMode(BLUE_LED,OUTPUT);
	pinMode(PHOTO_RESISTOR,INPUT);
	pinMode(PHOTO_POWER,OUTPUT);
	digitalWrite(PHOTO_POWER,HIGH);

	housekeepingThread = new Thread("Housekeeping", housekeepingTask);
}

//-------------------------------------------------------------------------------------------------
/*RGB_COLOR_BLUE : blue (0x000000ff)
RGB_COLOR_GREEN : green (0x0000ff00)
RGB_COLOR_CYAN : cyan (0x0000ffff)
RGB_COLOR_RED : red (0x00ff0000)
RGB_COLOR_MAGENTA : magenta (0x00ff00ff)
RGB_COLOR_YELLOW : yellow (0x00ffff00)
RGB_COLOR_WHITE : white (0x00ffffff)
RGB_COLOR_GRAY : gray (0x001f1f1f)
RGB_COLOR_ORANGE : orange (0x00ff6000)*/
void updateLED(uint16_t updateRate_ms) {
	RGB.control(true);

	if (globalData.cell.countdownTimer > 0) {
		uint16_t brightLevel = 0;

		if (globalData.cell.isValid) {
			brightLevel = globalData.cell.signalQuality*10;
			/*brightLevel = 255;*/
			if (globalData.cell.signalQuality < 5) {
				RGB.color(RGB_COLOR_BLUE);
			} else if (globalData.cell.signalQuality < 15) {
				RGB.color(RGB_COLOR_YELLOW);
			} else {
				RGB.color(RGB_COLOR_GREEN);
			}
		} else {
			brightLevel = 128;
			RGB.color(RGB_COLOR_RED);
		}
		RGB.brightness(brightLevel);
	}
}

//-------------------------------------------------------------------------------------------------
void i2c_scan() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
