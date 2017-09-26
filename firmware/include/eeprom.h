//-----------------------------------

typedef struct {
	bool initialized;
	struct {
		time_t		totalTime;
		uint16_t	powerCycles;
	} system;
} configurationDataType;

//==================================================================================================
class Eeprom {
 public:
	Eeprom();
	void update();		// Write eeprom if contents of configurationData have changed
	void clear();		// Erase the entire eeprom
	void setDefaults();
private:
	uint16_t getConfigurationDataChecksum();
	void writeConfigurationData();
};

#ifdef GLOBAL
	Eeprom eeprom;
	configurationDataType configurationData;
	uint16_t eepromChecksum;	// The checksum immediately follows configurationData in the EEPROM
	Thread *eepromThread;
#endif

extern Eeprom eeprom;
extern configurationDataType configurationData;
extern uint16_t eepromChecksum;
extern Thread *eepromThread;
