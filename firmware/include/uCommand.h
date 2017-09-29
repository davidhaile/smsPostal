/* _____CLASS DEFINITIONS____________________________________________________ */
/**

*/

#define MAX_RECEIVED_MESSAGES	50
#define SMS_BUFFER_SIZE			100
#define SMS_PHONE_SIZE			20
class uCommand {
  public:
	bool setDebug(bool);
	int setSMSMode(char);
	int sendMessage(char*, char*, int);
	int checkMessages(int);
	int deleteMessage(char, int);
	bool debugMode = false;
	unsigned char sendIndex = 0;
	unsigned char sendLength = 0;
	unsigned char rxIndex = 0;
	unsigned char rxLength = 0;
	char sendBuffer[SMS_BUFFER_SIZE];
	char rxBuffer[SMS_BUFFER_SIZE];
	struct smsStruct {
		char mess;
		char status[10];
		char phone[SMS_PHONE_SIZE];
		char sms[SMS_BUFFER_SIZE];
	};
	struct smsStruct* smsPtr;
	struct smsStruct smsResults[MAX_RECEIVED_MESSAGES];
	char numMessages;
  private:
};
