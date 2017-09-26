// SMS
#include "include/environ.h"

Thread *smsThread;

//------------------------------------------------------------------------------------------------------
os_thread_return_t smsTask() {
	WAIT_UNTIL_SYSTEM_IS_READY;

	while (true) {
		delay(ONE_MINUTE);
	}
}
