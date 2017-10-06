//--------------------------------------------------------------------------------------------------
// SMS
//--------------------------------------------------------------------------------------------------
/*
	A customer is identified by a phone number. The customer subscribes to the list by texting the
	command "add". The customer is removed by texting the word "delete" or "remove". A customer
	may also text the command "list" and receive a list of customers in response. TBD: Figure this
	out when return is greater than 140 characters.
*/
#include "include/environ.h"

#ifndef DISABLE_CELL
	#define USE_UCOMMAND
#endif

Thread *smsThread;

#ifdef USE_UCOMMAND
	uCommand uCmd;
#else
	static int callback(int, const char *, int, char *);
#endif

char szReturn[32] = "";

//--------------------------------------------------------------------------------------------------
os_thread_return_t smsTask() {
	/*uint16_t counter = 0;*/
	WAIT_UNTIL_SYSTEM_IS_READY;

	// Wait even more time
	delay(FIFTEEN_SECONDS);

	sms.mutex = false;

	while (true) {
		sms.update();
		delay(ONE_SECOND);
	}
}

//--------------------------------------------------------------------------------------------------
void Sms::update() {
	static bool toggle = false;

	/*WITH_LOCK(Serial) {
		Serial.print("SMS Task: ");
		Serial.print(counter);
		Serial.print(", ");
		Serial.print(sms.smsCounter);
		Serial.print(", ");
		Serial.print(sms.smsDelete);

		Serial.println();
		counter++;
	}
*/
	sms.check();

	if (sms.requestDeleteAll) {
		sms.requestDeleteAll = false;

		sms.deleteAll();
	}

	// This may be too much for a long test. Fills the screen with nothing useful.
	Serial.print('.');

	LED(toggle);
	toggle = !toggle;
}

//--------------------------------------------------------------------------------------------------
void Sms::test() {
	Serial.println("Sending a message to my own phone");
	sendMessage((char *)"9706912766", (char *)"Test Message");
	/*uCmd.sendMessage((char *)"9706912766", (char *)"Test Message", 5000);*/
}

//--------------------------------------------------------------------------------------------------
void Sms::check() {
#ifdef USE_UCOMMAND
	char incomingMessage[SMS_BUFFER_SIZE];
	char phoneNumber[SMS_PHONE_SIZE];

	/*if (uCmd.checkMessages(ONE_SECOND) == RESP_OK) {*/
	if (uCmd.checkMessages(TEN_SECONDS) == RESP_OK) {
		uCmd.smsPtr = uCmd.smsResults;
		for(int i=0;i<uCmd.numMessages;i++){
			if ((strlen(uCmd.smsPtr->status) > 0) && strstr(uCmd.smsPtr->status, "UNREAD")) {
				strncpy(incomingMessage, uCmd.smsPtr->sms, sizeof(incomingMessage));
				strncpy(phoneNumber, uCmd.smsPtr->phone, sizeof(phoneNumber));

				WITH_LOCK(Serial) {
					displayTime();
					Serial.printlnf("Message %d: From: %s [%s] Status: [%s]", i, phoneNumber, incomingMessage, uCmd.smsPtr->status);
				}

				// Process the incoming command
				// TBD: Change all this so that an incoming command such as "add", adds the senders phone number.
				/*sCmd.processMessage(incomingMessage, phoneNumber);*/
				/*sCmd.processCommand(incomingMessage);*/

				uCmd.smsPtr++;
				smsCounter++;

				// Delete everything that arrived because it has already been processed
				// 2017-09-29 This may not be necessary
				sms.requestDeleteAll = true;
			}
		}
	}
#endif
}

//--------------------------------------------------------------------------------------------------
void Sms::deleteAll() {
#ifdef USE_UCOMMAND
	// delete all the messages
	uCmd.smsPtr = uCmd.smsResults;
	for(int i=0;i<uCmd.numMessages;i++){
		uCmd.deleteMessage(uCmd.smsPtr->mess,10000);
		/*WITH_LOCK(Serial) {
			if(uCmd.deleteMessage(uCmd.smsPtr->mess,10000) == RESP_OK){
				Serial.println("message deleted successfully");
			} else {
				Serial.println("could not delete message");
			}
		}*/
		uCmd.smsPtr++;
		smsDelete++;
	}
#endif
}

//--------------------------------------------------------------------------------------------------
void Sms::add(char *phoneNumber) {
	customerList.add(phoneNumber);
}

//--------------------------------------------------------------------------------------------------
void Sms::remove(char *phoneNumber) {
	customerList.remove(phoneNumber);
}

//--------------------------------------------------------------------------------------------------
void Sms::list() {
	customerList.list();
}

//--------------------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------------------
Sms::Sms() {
	#ifdef USE_UCOMMAND
		uCmd.setDebug(true);
		/*uCmd.setDebug(false);*/

		// set up text mode for the sms
		uCmd.setSMSMode(1);
	#endif

	#ifndef DISABLE_THREADS
		smsThread = new Thread("SMS", smsTask);
	#endif
}


#ifndef USE_UCOMMAND
	//--------------------------------------------------------------------------------------------------
	static int callback(int type, const char* buf, int len, char* param) {
		WITH_LOCK(Serial) {
			/*Serial.print("Return: ");*/
			Serial.write((const uint8_t*)buf, len);
			/*Serial.println();*/
		}

		return WAIT;
	}
#endif

//--------------------------------------------------------------------------------------------------
// Phone number must have the area code in it. Ex: "9706912766"
//--------------------------------------------------------------------------------------------------
int Sms::sendMessage(char *inputPhoneNumber, char* pMessage) {
	int retVal = RESP_OK;
	char phoneNumber[MAX_PHONE_NUMBER];

	// Validate number
	int length = strlen(inputPhoneNumber);

	if (length < 7) {
		// Not enough numbers
		return(FAIL);
	}

	if (length == 10) {
		// Missing '1' at the beginning
		strcpy(phoneNumber, "1");
		strncat(phoneNumber, inputPhoneNumber, sizeof(phoneNumber) - 1);
	} else {
		// Assume all is OK
		strncpy(phoneNumber, inputPhoneNumber, sizeof(phoneNumber));
	}

	#ifdef USE_UCOMMAND
		uCmd.sendMessage(inputPhoneNumber, pMessage, TIMEOUT);
	#else
		char szCmd[80];
		memset(szCmd, 0, sizeof(szCmd));
		sprintf(szCmd, "AT+CMGS=\"+%s\",145\r\n", phoneNumber);

		char szReturn[32] = "";

		GRAB_MUTEX;
		Cellular.command(callback, szReturn, TIMEOUT, "AT+CMGF=1\r\n");
		Cellular.command(callback, szReturn, TIMEOUT, szCmd);
		Cellular.command(callback, szReturn, TIMEOUT, pMessage);
		RELEASE_MUTEX;

		sprintf(szCmd, "%c", CTRL_Z);

		retVal = Cellular.command(callback, szReturn, TIMEOUT, szCmd);
	#endif

	return retVal;
}
