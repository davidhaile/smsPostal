//-----------------------------------

#define MAX_PHONE_NUMBER    14
#define CTRL_Z				0x1A
#define TIMEOUT				10000
#define ERASED_VALUE		0xFF

#ifdef DISABLE_CELL
	#define REPLY_VIA_SERIAL_PORT
#else
	#define REPLY_VIA_SERIAL_PORT
#endif

#ifdef REPLY_VIA_SERIAL_PORT
	#define REPLY_MESSAGE(a)	Serial.println((char *)a)
#else
	#define REPLY_MESSAGE(a)	sms.sendMessage(phoneNumber, (char *)a)
#endif

//--------------------------------------------------------------------------------------------------
class Sms {
  public:
	Sms();
	int sendMessage(char *, char*);
	void add(char *);
	void remove(char *);
	void list();
  private:
};

#ifdef GLOBAL
	Sms sms;
#endif

extern Sms sms;

//--------------------------------------------------------------------------------------------------
struct Node {
	char phoneNumber[MAX_PHONE_NUMBER];
	Node *next;
};

class List {
  private:
	Node *head, *tail;
  public:
	List() {
		head = NULL;
		tail = NULL;
		readList();
	}

	// Write FF's to all available EEPROM
	void clearList() {
		while (head!=NULL) {
			delete_last();
		}
	}

	// Store the customer list in EEPROM
	bool storeList() {
		uint16_t startAddress = eeprom.available.startAddress;
		char tempPhoneNumber[MAX_PHONE_NUMBER];
		int counter = 0;

		Node *current = head;
		while (current!=NULL) {
			#error CRAPOLA Stopped here
			WITH_LOCK(Serial) {
				Serial.print("Adding \"");
				Serial.print(current->phoneNumber);
				Serial.println("\"");
			}
			memset(tempPhoneNumber, 0, sizeof(tempPhoneNumber));
			strcpy(tempPhoneNumber, current->phoneNumber);
			EEPROM.put(startAddress, tempPhoneNumber);
			startAddress += sizeof(tempPhoneNumber);
			current = current->next;
			counter++;
		}

		WITH_LOCK(Serial) {
			Serial.print("The list is ");
			Serial.print(counter);
			Serial.println(" entries.");
		}

		return(PASS);
	}

	//----------------------------------------------------------------------------------------------
	bool readList() {
		uint16_t startAddress = eeprom.available.startAddress;
		char tempPhoneNumber[MAX_PHONE_NUMBER];
		uint16_t counter = 0;

		do {
			EEPROM.get(startAddress, tempPhoneNumber);
			if (tempPhoneNumber[0] != ERASED_VALUE) {
				WITH_LOCK(Serial) {
					Serial.print(counter);
					Serial.print(": ");
					Serial.println(tempPhoneNumber);
				}

				// Add to list if it doesn't exist
				if (find(tempPhoneNumber) == -1) {
					add(tempPhoneNumber);
				}
				startAddress += sizeof(tempPhoneNumber);
				counter++;
			}
		} while (tempPhoneNumber[0] != ERASED_VALUE);

		WITH_LOCK(Serial) {
			Serial.print("The list is ");
			Serial.print(counter);
			Serial.println(" entries.");
		}

		return(PASS);
	}

	//----------------------------------------------------------------------------------------------
	// Add the value to the list.
	// Return FAIL if there is not enough space.
	// Return FAIL if the number already exists in the list.
	// TBD: "1234567" should be considered the same as "3071234567" and "13071234567"
	//----------------------------------------------------------------------------------------------
	bool add(char *phoneNumber) {
		Node *temp = new Node;

		Serial.print("Adding: \"");
		Serial.print(phoneNumber);
		Serial.println("\"");

		if (find(phoneNumber) >= 0) {
			REPLY_MESSAGE("This number is already in the list");
			return(FAIL);
		}

		strcpy(temp->phoneNumber, phoneNumber);
		temp->next = NULL;
		if (head == NULL) {
			head = temp;
			tail = temp;
			temp = NULL;
		} else {
			tail->next = temp;
			tail = temp;
		}

		storeList();

		REPLY_MESSAGE("Added");
		return(PASS);
	}

	//----------------------------------------------------------------------------------------------
	int numberOfEntries() {
		Node *current=new Node;
		current=head;
		int counter = 0;

		while (current != NULL) {
			current = current->next;
		}

		return(counter);
	}
	//----------------------------------------------------------------------------------------------
	// Returns -1 if phoneNumber is not found, otherwise returns its position in the list
	//----------------------------------------------------------------------------------------------
	int find(char *phoneNumber) {
		Node *current=new Node;
		current=head;

		int counter = 0;
		bool foundNumber = false;

		// Serial.print(phoneNumber);
		// Serial.print(": ");

		while ((current!=NULL) && !foundNumber) {
			// Serial.print(current->phoneNumber);
			// Serial.print(", ");
			int result = strcmp(current->phoneNumber, phoneNumber);
			// Serial.print("Result: "); Serial.print(result);
			if (result == 0) {
				foundNumber = true;
			} else {
				current=current->next;
			}
			counter++;
		}

		if (foundNumber) {
			// Serial.println("Found");
			return(counter);
		} else {
			// Serial.println("Not Found");
			return(-1);
		}
	}

	//----------------------------------------------------------------------------------------------
	// Find the value and remove it. Return PASS or FAIL.
	//----------------------------------------------------------------------------------------------
	bool remove(char *phoneNumber) {
		int position = find(phoneNumber);

		if (position >= 0) {
			delete_position(position);
			REPLY_MESSAGE("Removed");
			storeList();
			return(PASS);
		} else {
			return(FAIL);
		}
	}

	//----------------------------------------------------------------------------------------------
	void list() {
		Node *temp=new Node;
		temp=head;
		int counter = 0;

		WITH_LOCK(Serial) {
			Serial.println("Start");
			while (temp!=NULL) {
				Serial.print(counter);
				Serial.print(": ");
				Serial.println(temp->phoneNumber);
				temp=temp->next;
				counter++;
			}
			Serial.println("Done");
		}
	}

	//----------------------------------------------------------------------------------------------
	void createnode(char *phoneNumber) {
		Node *temp = new Node;
		strcpy(temp->phoneNumber, phoneNumber);
		temp->next = NULL;
		if (head == NULL) {
			head = temp;
			tail = temp;
			temp = NULL;
		} else {
			tail->next = temp;
			tail = temp;
		}
	}
	//----------------------------------------------------------------------------------------------
	void insert_start(char *phoneNumber) {
		Node *temp = new Node;
		strcpy(temp->phoneNumber, phoneNumber);
		temp->next = head;
		head = temp;
	}

	//----------------------------------------------------------------------------------------------
	void insert_position(int pos, char *phoneNumber) {
		Node *pre = new Node;
		Node *cur = new Node;
		Node *temp = new Node;
		cur = head;
		for (int i = 1; i < pos; i++) {
			pre = cur;
			cur = cur->next;
		}
		strcpy(temp->phoneNumber, phoneNumber);
		pre->next = temp;
		temp->next = cur;
	}

	//----------------------------------------------------------------------------------------------
	void delete_first() {
		Node *temp = new Node;
		temp = head;
		head = head->next;
		delete temp;
	}

	//----------------------------------------------------------------------------------------------
	void delete_last() {
		Node *current = new Node;
		Node *previous = new Node;
		current = head;
		while (current->next != NULL) {
			previous = current;
			current = current->next;
		}
		tail = previous;
		previous->next = NULL;
		delete current;
	}

	//----------------------------------------------------------------------------------------------
	void delete_position(int pos) {
		Node *current = new Node;
		Node *previous = new Node;
		current = head;

		if (pos == 0) {
			delete_first();
		} else {
			for (int i = 1; i < pos; i++) {
				previous = current;
				current = current->next;
			}
			previous->next = current->next;
			delete current;	// Fixed
		}
	}
};

#ifdef GLOBAL
	List customerList;
#endif

extern List customerList;
