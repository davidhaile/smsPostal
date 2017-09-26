// Environ.h

#ifdef NULL
	#undef NULL
#endif
#define NULL	(void *)0

#define PASS	false
#define FAIL	true

// System Includes
#include "Particle.h"
#include <sys/time.h>
#include "SerialCommand/SerialCommand.h"
#include "elapsedMillis/elapsedMillis.h"
#include <math.h>

#include "hardwareProfile.h"
#include "build.h"

// Project Includes
#include "macros.h"
#include "globalData.h"
#include "utils.h"
#include "commands.h"
#include "housekeeping.h"
#include "eeprom.h"
#include "sms.h"

#define WAIT_UNTIL_SYSTEM_IS_READY	while (!globalData.system.ready) {delay(500);};

#define FEET_PER_METER		3.28084

#define HOURS_CONVERSION	(1/(1000.0*60*60))
#define FIVE_SECONDS		(5*1000)
#define FIFTEEN_SECONDS		(15*1000)
#define ONE_MINUTE			(60*1000)
#define FIVE_MINUTES    	(5*60*1000)
#define FIFTEEN_MINUTES    	(15*60*1000)
#define ONE_HOUR	    	(60*60*1000)
#ifdef MQTT_DEBUG
	#define LONG_UDPATE_TIME_MS  FIFTEEN_SECONDS
	#define SHORT_UDPATE_TIME_MS (5*1000)
#else
	// #define LONG_UDPATE_TIME_MS  FIVE_MINUTES
	#define LONG_UDPATE_TIME_MS  FIFTEEN_MINUTES
	#define SHORT_UDPATE_TIME_MS FIFTEEN_SECONDS
#endif
