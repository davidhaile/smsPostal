//-----------------------------------

class Sms {
public:
	Sms();
	int sendMessage(char *, char*);
private:
};

#ifdef GLOBAL
	Sms sms;
#endif

extern Sms sms;
