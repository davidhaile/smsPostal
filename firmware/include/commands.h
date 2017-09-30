#include <SerialCommand/SerialCommand.h>

extern SerialCommand sCmd;

// Must be less than 256
#define SERIALPORT_BUFFER_SIZE	120

void updateCommandList();

class Commands {
public:
	Commands();
private:
};

#ifdef GLOBAL
	Commands commands;
#endif

extern Commands commands;
