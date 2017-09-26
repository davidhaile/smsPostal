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

Thread *smsThread;

static int callback(int, const char *, int, char *);

//--------------------------------------------------------------------------------------------------
os_thread_return_t smsTask() {
	WAIT_UNTIL_SYSTEM_IS_READY;

	while (true) {
		delay(ONE_MINUTE);
	}
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
	smsThread = new Thread("SMS", smsTask);
}

//--------------------------------------------------------------------------------------------------
static int callback(int type, const char* buf, int len, char* param) {
	WITH_LOCK(Serial) {
		Serial.print("Return: ");
		Serial.write((const uint8_t*)buf, len);
		Serial.println();
	}

	return WAIT;
}

//--------------------------------------------------------------------------------------------------
// Phone number must have the area code in it. Ex: "9706912766"
//--------------------------------------------------------------------------------------------------
int Sms::sendMessage(char *inputPhoneNumber, char* pMessage) {
	char szCmd[64];
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

	sprintf(szCmd, "AT+CMGS=\"+%s\",145\r\n", phoneNumber);

	WITH_LOCK(Serial) {
		Serial.print("Sending command ");
		Serial.print(szCmd);
		Serial.println();
	}

	char szReturn[32] = "";

	Cellular.command(callback, szReturn, TIMEOUT, "AT+CMGF=1\r\n");
	Cellular.command(callback, szReturn, TIMEOUT, szCmd);
	Cellular.command(callback, szReturn, TIMEOUT, pMessage);

	sprintf(szCmd, "%c", CTRL_Z);

	retVal = Cellular.command(callback, szReturn, TIMEOUT, szCmd);

	WITH_LOCK(Serial) {
		if (retVal == RESP_OK) {
			Serial.println("+OK, Message Send");
		} else {
			Serial.println("+ERROR, error sending message");
		}
	}

	return retVal;
}
