#include "include/environ.h"

static Thread *commandThread;

SerialCommand sCmd;

static void sendHelp();
static void ping();
static void unrecognized(const char *);
static void update();
static void smsAdd();
static void smsRemove();
static void smsList();

typedef struct {
	/*char *command;*/
	String command;
	void (* process)();
} commandListType;

commandListType commandList[] = {
	// SMS Commands
	{"add", 	smsAdd},
	{"del", 	smsRemove},
	{"rem", 	smsRemove},
	{"delete", 	smsRemove},
	{"remove", 	smsRemove},
	{"list", 	smsList},

	{"on", 		LED_on},
	{"off", 	LED_off},
	{"ping",	ping},

	{"time",	displayTime},
	{"h", 		sendHelp},	// Must be last in the list
};

#define NUMBER_OF_COMMANDS  sizeof(commandList)/sizeof(commandListType)

//------------------------------------------------------------------------------------------------------
void smsList() {
	sms.list();
}

//------------------------------------------------------------------------------------------------------
void smsAdd() {
	// TBD: Get this number from the next value
	sms.add((char *)"9706912766");
}

//------------------------------------------------------------------------------------------------------
void smsRemove() {
	// TBD: Get this number from the next value
	sms.remove((char *)"9706912766");
}

//------------------------------------------------------------------------------------------------------
static void update() {
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
		update();
		delay(100);
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

	commandThread = new Thread("Commands", commandTask);
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
	WITH_LOCK(Serial) {
		Serial.print("Unrecognized command: ");
		char c = *command;
		Serial.println(c);
	}
}

//------------------------------------------------------------------------------------------------------
static void sendHelp() {
	WITH_LOCK(Serial) {
		Serial.println("Commands");
		Serial.println("--------");
		for (uint16_t i=0; i<(NUMBER_OF_COMMANDS-1); i++) {
			Serial.println(commandList[i].command);
		}
	}
}
