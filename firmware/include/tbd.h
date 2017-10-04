//-------------------------------------------------------------------------------------------------
// TBD.h
//-------------------------------------------------------------------------------------------------

/*
	TBD:

	2017-10-04: Create an smsTest function to test transmit.

	2017-09-27: Having trouble with an extra number added to the list that is a NULL length.

	2017-09-29: No reason to read a text if it has already been read. Check the status for UA_READ.
	REC UNREAD	REC UNREAD+19706912766
	REC READAD	REC READAD+19706912766

	2017-09-29: SerialCommand should be modified to allow multiple terminators and ignore all in
	message. '\r'. '\n', '\0'

	Why does it stop accepting Serial input? TBD: Add bufferIncomingValue() from other projects.

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

*/
