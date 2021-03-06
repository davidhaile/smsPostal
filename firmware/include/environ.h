// Environ.h

// During initial setup of a device these are the usual LED specifications:
// White pulse: Start-up (happens when the Electron is first powered on or when it's reset)
// Flashing blue: Listening mode.
// Flashing green: Connecting to cellular network
// Flashing cyan: Connecting to Particle Cloud. Connected to the network, but not necessarily the internet yet.
// High-speed flashing cyan: Particle Cloud handshake
// Breathing cyan: Connected to Particle Cloud
// Flashing magenta: Receiving new firmware update over-the-air (OTA)
// Breathing magenta Safe mode, connected to Particle Cloud but user firmware not running
//
// #ifdef NULL
// 	#undef NULL
// #endif
// #define NULL	(void *)0

#define NULL_CHAR	'\0'

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
#include "uCommand.h"

#define WAIT_UNTIL_SYSTEM_IS_READY	while (!globalData.system.ready) {delay(500);};

#define FEET_PER_METER		3.28084

#define HOURS_CONVERSION	(1/(1000.0*60*60))
#define ONE_SECOND			1000
#define FIVE_SECONDS		(5*ONE_SECOND)
#define TEN_SECONDS			(10*ONE_SECOND)
#define FIFTEEN_SECONDS		(15*ONE_SECOND)
#define ONE_MINUTE			(60*ONE_SECOND)
#define FIVE_MINUTES    	(5*60*ONE_SECOND)
#define FIFTEEN_MINUTES    	(15*60*ONE_SECOND)
#define ONE_HOUR	    	(60*60*ONE_SECOND)
#ifdef MQTT_DEBUG
	#define LONG_UDPATE_TIME_MS  FIFTEEN_SECONDS
	#define SHORT_UDPATE_TIME_MS (5*1000)
#else
	// #define LONG_UDPATE_TIME_MS  FIVE_MINUTES
	#define LONG_UDPATE_TIME_MS  FIFTEEN_MINUTES
	#define SHORT_UDPATE_TIME_MS FIFTEEN_SECONDS
#endif
