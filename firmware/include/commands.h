#include <SerialCommand/SerialCommand.h>

extern SerialCommand sCmd;

// Must be less than 256
#ifndef SERIALPORT_BUFFER_SIZE
	#define SERIALPORT_BUFFER_SIZE	64
#endif

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
