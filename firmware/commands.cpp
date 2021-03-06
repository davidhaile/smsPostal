#include "include/environ.h"

static Thread *commandThread;

SerialCommand sCmd;

static void sendHelp();
static void ping();
static void unrecognized(const char *);
static void smsAdd();
static void smsRemove();
static void smsList();
static void process_command();
static void smsReadList();
static void smsClearList();
static void analogDisplay();
static void smsCheck();
static void smsDeleteAll();
static void smsTest();
static void smsDebug();

typedef struct {
	char command[SERIALCOMMAND_MAXCOMMANDLENGTH];
	String description;
	void (* process)();
} commandListType;

commandListType commandList[] = {
	// SMS Commands
	{"add",		"Add Customer", 			smsAdd},
	{"del",		"Delete Customer", 			smsRemove},
	{"rem",		"Delete Customer", 			smsRemove},
	{"delete",	"Delete Customer", 			smsRemove},
	{"remove",	"Delete Customer", 			smsRemove},
	{"list",	"List customers", 			smsList},
	{"read",	"Read from EEPROM", 		smsReadList},
	{"clear",	"Clear customer list",		smsClearList},
	{"check",	"Check for incoming SMS",	smsCheck},
	{"x", 		"Delete all stored SMS",	smsDeleteAll},
	{"test",	"SMS Test",					smsTest},
	{"debug",	"Toggle debug mode",		smsDebug},

	{"on", 		"LED on",					LED_on},
	{"off", 	"LED off",					LED_off},
	{"p", 		"Ping",						ping},
	{"process",	"Process command",			process_command},
	{"light",	"Light display",			analogDisplay},

	{"time",	"Time",						displayTime},
	{"help", 	"Help",						sendHelp},
	{"h", 		"Help",						sendHelp},	// Must be last in the list
};

#define NUMBER_OF_COMMANDS  sizeof(commandList)/sizeof(commandListType)

//--------------------------------------------------------------------------------------------------
static void smsDebug() {
	static bool toggle = false;
	if (toggle) {
		Serial.println("Debug mode is OFF");
		uCmd.setDebug(false);
		toggle = false;
	} else {
		Serial.println("Debug mode is ON");
		uCmd.setDebug(true);
		toggle = true;
	}
}

//--------------------------------------------------------------------------------------------------
static void smsTest() {
	sms.test();
}

//--------------------------------------------------------------------------------------------------
static void smsCheck() {
	Serial.println("Check texts");
	sms.check();
	Serial.println("Done");
}

//--------------------------------------------------------------------------------------------------
static void smsDeleteAll() {
	Serial.println("Delete All outstanding texts");
	sms.requestDeleteAll = true;
	/*sms.deleteAll();*/
	Serial.println("Done");
}

//--------------------------------------------------------------------------------------------------
static void analogDisplay() {
	Serial.print("Ambient light ");
	Serial.print(globalData.system.ambientLight, 2);
	Serial.println();
}

//--------------------------------------------------------------------------------------------------
static void smsList() {
	Serial.println("List of numbers");
	sms.list();
}

//--------------------------------------------------------------------------------------------------
static void smsClearList() {
	Serial.println("Erasing available EEPROM");
	EEPROM.clear();
	eeprom.writeConfigurationData();
	customerList.clearList();
}

//--------------------------------------------------------------------------------------------------
static void smsReadList() {
	Serial.println("Reading the list from the EEPROM");
	customerList.readList();
}

//--------------------------------------------------------------------------------------------------
void smsAdd() {
	char *arg;

	arg = sCmd.next();
	if (arg != NULL) {
		// TBD: Check that it is a valid number. Add area code if none given.
		sms.add(arg);
	} else {
		Serial.println("No arguments");
	}
}

//--------------------------------------------------------------------------------------------------
void smsRemove() {
	char *arg;

	arg = sCmd.next();
	if (arg != NULL) {
		sms.remove(arg);
	}
}

//--------------------------------------------------------------------------------------------------
void Commands::update() {
	switch (sCmd.state) {
	case SC_INVALID_STATE:
		sCmd.state = SC_WAIT_FOR_COMMAND;
		break;
	case SC_WAIT_FOR_COMMAND:
		sCmd.readSerial();
		break;
	default:
		/*sCmd.state = SC_WAIT_FOR_COMMAND;*/
		break;
	}

#ifdef SKIP
	switch (sCmd.state) {
	case SC_INVALID_STATE:
		Serial.println("SC_INVALID_STATE");
		break;
	case SC_WAIT_FOR_COMMAND:
		Serial.println("SC_WAIT_FOR_COMMAND");
		break;
	case SC_WAIT_FOR_REQUEST:
		Serial.println("SC_WAIT_FOR_REQUEST");
		break;
	case SC_SEND_REQUEST:
		Serial.println("SC_SEND_REQUEST");
		break;
	case SC_WAIT_FOR_RESPONSE:
		Serial.println("SC_WAIT_FOR_RESPONSE");
		break;
	default:
		Serial.println("Unknown statel");
		break;
	}
#endif
}

//------------------------------------------------------------------------------------------------------
// Run continuously. Check for serial input once per second.
//------------------------------------------------------------------------------------------------------
os_thread_return_t commandTask() {
	WAIT_UNTIL_SYSTEM_IS_READY;

	while (true) {
		commands.update();
		delay(5);
	}
}

//------------------------------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------------------------------
Commands::Commands() {
	commandListType *pList = commandList;

	for (uint16_t i = 0; i < NUMBER_OF_COMMANDS; i++) {
		sCmd.addCommand(pList->command, pList->process);
		pList++;
	}

	sCmd.setDefaultHandler(unrecognized);

	#ifndef DISABLE_THREADS
		commandThread = new Thread("Commands", commandTask);
	#endif
}

//------------------------------------------------------------------------------------------------------
static void ping() {
	WITH_LOCK(Serial) {
		Serial.println("Ping");
	}
	globalData.system.pingRequest = true;
}

//------------------------------------------------------------------------------------------------------
// This gets set as the default handler, and gets called when no other command matches.
//------------------------------------------------------------------------------------------------------
static void unrecognized(const char *command) {
	/*WITH_LOCK(Serial) {
		Serial.print("Unrecognized command: ");
		char c = *command;
		Serial.println(c);
	}*/
}

//------------------------------------------------------------------------------------------------------
static void sendHelp() {
	WITH_LOCK(Serial) {
		Serial.println("Commands");
		Serial.println("--------");
		for (uint16_t i = 0; i < (NUMBER_OF_COMMANDS - 1); i++) {
			Serial.println(commandList[i].command);
		}
	}
}

//------------------------------------------------------------------------------------------------------
static void process_command() {
	int aNumber;
	char *arg;

	Serial.println("We're in process_command");
	arg = sCmd.next();
	if (arg != NULL) {
		aNumber = atoi(arg);  // Converts a char string to an integer
		Serial.print("First argument was: ");
		Serial.println(aNumber);
	} else {
		Serial.println("No arguments");
	}

	arg = sCmd.next();
	if (arg != NULL) {
		aNumber = atol(arg);
		Serial.print("Second argument was: ");
		Serial.println(aNumber);
	} else {
		Serial.println("No second argument");
	}
}
