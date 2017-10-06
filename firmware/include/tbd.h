//-------------------------------------------------------------------------------------------------
// TBD.h
//-------------------------------------------------------------------------------------------------
/*
*/


/*
	TBD:

	2017-10-06 3am Status:
	- It doesn't delete messages that it just read.
	- Sometimes the incoming message is returned as a null string. This may be specific to the
	  message, such as "Time".
	- If message has a length, it is correctly processed.
	- TBD: Add feature to add incoming message phone number to list.
	- SMS Transmit doesn't work and may screw up the device. It started breathing green, but otherwise
	  continued to work. Next debug step is transmit.

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
