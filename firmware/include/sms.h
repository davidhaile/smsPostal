//-----------------------------------

#define MAX_PHONE_NUMBER    14
#define CTRL_Z				0x1A
#define TIMEOUT				10000

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
// Single Linked List - TBD: Write the customerList structure to EEPROM and read on startup.
//--------------------------------------------------------------------------------------------------
class NodeType {
public:
	NodeType(char *inputNumber, NodeType *p) {
		strcpy(phoneNumber, inputNumber);
		next = p;
	}
	char phoneNumber[MAX_PHONE_NUMBER];
	NodeType *next;
};

//--------------------------------------------------------------------------------------------------
class List {
  private:
	nodeType *head, *tail;
  public:
	List() {
		head = NULL;
		tail = NULL;
	}

	//----------------------------------------------------------------------------------------------
	// Add the value to the list.
	// Return FAIL if there is not enough space.
	// Return FAIL if the number already exists in the list.
	// TBD: "1234567" should be considered the same as "3071234567" and "13071234567"
	//----------------------------------------------------------------------------------------------
	bool add(nodeType *list, char *phoneNumber) {
		node *temp = new node;

		Serial.print("Adding: ");
		Serial.println(phoneNumber);

		if (find(phoneNumber) >= 0) {
			REPLY_MESSAGE("This number is already in the list");
			return(FAIL);
		}

		temp->phoneNumber = phoneNumber;
		temp->next = NULL;
		if (head == NULL) {
			head = temp;
			tail = temp;
			temp = NULL;
		} else {
			tail->next = temp;
			tail = temp;
		}

		eeprom.updateCustomerList();

		REPLY_MESSAGE("Added");
		return(PASS);
	}

	//----------------------------------------------------------------------------------------------
	// Returns -1 if phoneNumber is not found, otherwise returns its position in the list
	//----------------------------------------------------------------------------------------------
	int find(char *phoneNumber) {
		node *current=new node;
		current=head;

		int counter = 0;
		bool foundNumber = false;

		Serial.print(phoneNumber);
		Serial.print(": ");

		while ((current!=NULL) && !foundNumber) {
			Serial.print(current->phoneNumber);
			Serial.print(", ");
			int result = strcmp(current->phoneNumber, phoneNumber);
			Serial.print("Result: "); Serial.print(result);
			if (result == 0) {
				foundNumber = true;
			} else {
				current=current->next;
			}
			counter++;
		}

		if (foundNumber) {
			Serial.println("Found");
			return(counter);
		} else {
			Serial.println("Not Found");
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
			eeprom.updateCustomerList();
			return(PASS);
		} else {
			return(FAIL);
		}
	}

	//----------------------------------------------------------------------------------------------
	void list() {
		node *temp=new node;
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
		node *temp = new node;
		temp->phoneNumber = phoneNumber;
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
		node *temp = new node;
		temp->phoneNumber = phoneNumber;
		temp->next = head;
		head = temp;
	}

	//----------------------------------------------------------------------------------------------
	void insert_position(int pos, char *phoneNumber) {
		node *pre = new node;
		node *cur = new node;
		node *temp = new node;
		cur = head;
		for (int i = 1; i < pos; i++) {
			pre = cur;
			cur = cur->next;
		}
		temp->phoneNumber = phoneNumber;
		pre->next = temp;
		temp->next = cur;
	}

	//----------------------------------------------------------------------------------------------
	void delete_first() {
		node *temp = new node;
		temp = head;
		head = head->next;
		delete temp;
	}

	//----------------------------------------------------------------------------------------------
	void delete_last() {
		node *current = new node;
		node *previous = new node;
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
		node *current = new node;
		node *previous = new node;
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
