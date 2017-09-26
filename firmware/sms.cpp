// SMS
#include "include/environ.h"

Thread *smsThread;

#define MAX_PHONE_NUMBER    14
#define CTRL_Z 0x1A
#define TIMEOUT 10000

static int callback(int, const char *, int, char *);

//--------------------------------------------------------------------------------------------------
os_thread_return_t smsTask() {
	WAIT_UNTIL_SYSTEM_IS_READY;

	while (true) {
		delay(ONE_MINUTE);
	}
}

//--------------------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------------------
Sms::Sms() {
	smsThread = new Thread("SMS", smsTask);
}

//--------------------------------------------------------------------------------------------------
static int callback(int type, const char* buf, int len, char* param) {
    Serial.print("Return: ");
    Serial.write((const uint8_t*)buf, len);
    Serial.println();

    return WAIT;
}

//--------------------------------------------------------------------------------------------------
// Phone number must have the area code in it. Ex: "9706912766"
//--------------------------------------------------------------------------------------------------
int Sms::sendMessage(char *phoneNumber, char* pMessage) {
    char szCmd[64];

    sprintf(szCmd, "AT+CMGS=\"+%s\",145\r\n", phoneNumber);

    Serial.print("Sending command ");
    Serial.print(szCmd);
    Serial.println();

    char szReturn[32] = "";

    Cellular.command(callback, szReturn, TIMEOUT, "AT+CMGF=1\r\n");
    Cellular.command(callback, szReturn, TIMEOUT, szCmd);
    Cellular.command(callback, szReturn, TIMEOUT, pMessage);

    sprintf(szCmd, "%c", CTRL_Z);

    int retVal = Cellular.command(callback, szReturn, TIMEOUT, szCmd);

    if(RESP_OK == retVal){
        Serial.println("+OK, Message Send");
    }
    else{
        Serial.println("+ERROR, error sending message");
    }

    return retVal;
}
