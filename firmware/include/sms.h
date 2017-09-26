//-----------------------------------

class Sms {
public:
	Sms() {
		smsThread = new Thread("SMS", smsTask);
	}
private:
};

#ifdef GLOBAL
	Sms sms;
#endif

extern Sms sms;
