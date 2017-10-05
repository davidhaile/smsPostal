//-------------------------------------------------------------------------------------------------
// build.h
//-------------------------------------------------------------------------------------------------
#ifndef BUILD_H
	#define BUILD_H

	// #define MY_DEBUG
	#ifdef MY_DEBUG
		#define SAVE_POWER
		#define STARTUP_WITH_CELL_OFF
	#else
	#endif

	// #define DISABLE_CELL
	#ifdef DISABLE_CELL
		#define SAVE_POWER
		#define STARTUP_WITH_CELL_OFF
	#endif

	// 2017-10-05 Trying to figure out why SMS isn't reliable
	#define DISABLE_THREADS

#endif
