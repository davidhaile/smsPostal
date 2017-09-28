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
#endif

static int callback(int, const char *, int, char *);
static int rxCallback(int, const char *, int, char *);

char szReturn[32] = "";

static int rxCallback(int type, const char* buf, int len, char* param) {
	WITH_LOCK(Serial) {
		Serial.print("Rx: ");
		Serial.write((const uint8_t*)buf, len);
		Serial.println();
	}

	return WAIT;
}

//--------------------------------------------------------------------------------------------------
os_thread_return_t smsTask() {
	uint16_t counter = 0;
	WAIT_UNTIL_SYSTEM_IS_READY;

	// Wait even more time
	delay(FIFTEEN_SECONDS);

	sms.mutex = false;

	while (true) {
		WITH_LOCK(Serial) {
			Serial.print("SMS Task: ");
			Serial.print(counter);
			Serial.print(", ");
			Serial.print(sms.smsCounter);
			Serial.println();
			counter++;
		}

		#ifdef SKIP	//DISABLE_CELL
			#ifdef USE_UCOMMAND
				GRAB_MUTEX;
				/*SINGLE_THREADED_BLOCK() {*/
				{

				}
				RELEASE_MUTEX;
			#else
				/*Cellular.command(rxCallback, szReturn, TIMEOUT, "AT+CMGR=1\r\n");*/
				Cellular.command(rxCallback, szReturn, TIMEOUT, "AT+CPMS?\r\n");	// Sort of. Need to understand what it is doing.
				/*Cellular.command(rxCallback, szReturn, TIMEOUT, "AT+CPMS=1\r\n");*/	// No
			#endif
		#endif

		delay(FIFTEEN_SECONDS);
	}
}

//--------------------------------------------------------------------------------------------------
void Sms::check() {
	uCmd.setDebug(false);
	if (uCmd.checkMessages(FIFTEEN_SECONDS) == RESP_OK) {
		uCmd.smsPtr = uCmd.smsResults;
		for(int i=0;i<uCmd.numMessages;i++){
			/*WITH_LOCK(Serial) {
				Serial.printlnf("message received %s",uCmd.smsPtr->sms);
			}*/
			uCmd.smsPtr++;
			smsCounter++;
		}
	}
	uCmd.setDebug(true);
}

//--------------------------------------------------------------------------------------------------
void Sms::deleteAll() {
	// delete all the messages
	uCmd.setDebug(false);
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
	}
	uCmd.setDebug(true);
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

	smsThread = new Thread("SMS", smsTask);
}

//--------------------------------------------------------------------------------------------------
static int callback(int type, const char* buf, int len, char* param) {
	WITH_LOCK(Serial) {
		/*Serial.print("Return: ");*/
		Serial.write((const uint8_t*)buf, len);
		/*Serial.println();*/
	}

	return WAIT;
}

//--------------------------------------------------------------------------------------------------
// Phone number must have the area code in it. Ex: "9706912766"
//--------------------------------------------------------------------------------------------------
int Sms::sendMessage(char *inputPhoneNumber, char* pMessage) {
	char szCmd[80];
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

	return retVal;
}
