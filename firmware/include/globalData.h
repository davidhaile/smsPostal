//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// globalData.h
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
typedef struct {
	bool ready;			// true when everything is initialized
	bool okToTransmit;
	bool statusUpdateRequest;
	bool pingRequest;
	time_t startTime;	// Time when powered up
	time_t totalTime;	// Total time that this device has had power applied. Starting with
	time_t uptime;		// Time since powerup
	float ambientLight;	// 0-100%
} systemDataType;

//-------------------------------------------------------------------------------------------------
typedef struct {
	bool isValid;
	float rssi;
	float signalQuality;
	uint16_t countdownTimer;	// LED Status on startup until zero
} cellDataType;

//-------------------------------------------------------------------------------------------------
typedef struct {
	systemDataType system;
	struct {
		float batteryChargePercent;
	} hardware;
	cellDataType cell;
} globalDataType;

#ifdef GLOBAL
	globalDataType globalData;
#else
	extern globalDataType globalData;
#endif
