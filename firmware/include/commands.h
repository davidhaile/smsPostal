#include <SerialCommand/SerialCommand.h>

extern SerialCommand sCmd;

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
