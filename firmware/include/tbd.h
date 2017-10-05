//-------------------------------------------------------------------------------------------------
// TBD.h
//-------------------------------------------------------------------------------------------------

/*
	TBD:

	2017-10-05: Status - It may have been OK until I tried to send a text, then it went into the
	constantly searching for a tower mode.

	2017-09-29: No reason to read a text if it has already been read. Check the status for UA_READ.
	REC UNREAD	REC UNREAD+19706912766
	REC READAD	REC READAD+19706912766

	Resources:

	Tips on sleep/wake of Electron with cell connection
	https://community.particle.io/t/particle-electron-pmic/31412/2

	How to send SMS from an Electron using AT commands
	https://community.particle.io/t/sending-sms-from-particle-electron-using-at-commands/21020

	Good read on system threading
	https://docs.particle.io/reference/firmware/electron/#system-thread

	Example of receiving SMS texts using AT commands
	https://community.particle.io/t/help-needed-to-split-incoming-sms/34275/4

	Great resource for workflow - compiling in the cloud, download via USB, etc.
	https://community.particle.io/t/local-compile-electron-workflow/21694/9

	How to handle cellular connection
	https://community.particle.io/t/how-to-handle-cellular-connection-loss-on-electron/31925

	Cricket Wireless APN Information
	https://www.cricketwireless.com/support/apps-and-services/bring-your-own-device-byod/customer/bring-your-own-android.html

	Implement serial port using interrupts
*/

/*
	DONE

	2017-10-05: Added ability to not use threading. It doesn't seem to have any affect on SMS reliability.
	It must be something else. The RGB LED says it isn't finding the tower.

*/
