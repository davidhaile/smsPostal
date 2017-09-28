#include "include/environ.h"

//------------------------------------------------------------------------------------------------------
os_thread_return_t eepromTask() {
	WAIT_UNTIL_SYSTEM_IS_READY;

	while (true) {
		eeprom.update();
		delay(ONE_MINUTE);
	}
}

//------------------------------------------------------------------------------------------------------
Eeprom::Eeprom() {
	EEPROM.get(0, configurationData);
	EEPROM.get(sizeof(configurationData), eepromChecksum);

	uint16_t _checksum = getConfigurationDataChecksum();
	if (_checksum != eepromChecksum) {
		setDefaults();
	}
	configurationData.system.powerCycles++;

	eepromThread = new Thread("EEPROM", eepromTask);
};

//------------------------------------------------------------------------------------------------------
uint16_t Eeprom::getConfigurationDataChecksum() {
	#define CHECKSUM_SEED	0xA5A5
    uint16_t chk = CHECKSUM_SEED;
	uint8_t *p = (uint8_t *)&configurationData;
	uint16_t length = sizeof(configurationData);

    while (length-- != 0) {
        chk -= *p++;
	}
    return chk;
}

//------------------------------------------------------------------------------------------------------
void Eeprom::setDefaults(){
	EEPROM.clear();

	memset(&configurationData, 0, sizeof(configurationData));

	writeConfigurationData();
}

//------------------------------------------------------------------------------------------------------
void Eeprom::writeConfigurationData() {
	eepromChecksum = getConfigurationDataChecksum();
	EEPROM.put(0, configurationData);
	EEPROM.put(sizeof(configurationData), eepromChecksum);

	available.startAddress = sizeof(configurationData) + sizeof(eepromChecksum);
	available.length = EEPROM.length() - available.startAddress;
}

//------------------------------------------------------------------------------------------------------
// If configurationData has changed, write it to the EEPROM
//------------------------------------------------------------------------------------------------------
void Eeprom::update() {
	uint16_t _checksum = getConfigurationDataChecksum();
	if (_checksum != eepromChecksum) {
		writeConfigurationData();
	}
}

//------------------------------------------------------------------------------------------------------
void Eeprom::clear() {
	Serial.println("Clearing eeprom");
	EEPROM.clear();
}
