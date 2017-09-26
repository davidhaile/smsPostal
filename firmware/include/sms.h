//-----------------------------------

#define MAX_PHONE_NUMBER    14
#define CTRL_Z				0x1A
#define TIMEOUT				10000

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

//--------------------------------------------------------------------------------------------------
// Single Linked List - TBD: Write the customerList structure to EEPROM and read on startup.
//--------------------------------------------------------------------------------------------------
struct node {
	char *phoneNumber;
	node *next;
};
class List {
  private:
	node *head, *tail;
  public:
	List() {
		head = NULL;
		tail = NULL;
	}

	//----------------------------------------------------------------------------------------------
	// Add the value to the list.
	// Return FAIL if there is not enough space.
	// Return FAIL if the number already exists in the list.
	//----------------------------------------------------------------------------------------------
	bool add(char *phoneNumber) {
		node *temp = new node;

		if (find(phoneNumber) >= 0) {
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

		while ((current!=NULL) && !foundNumber) {
			if (strcmp(current->phoneNumber, phoneNumber) == 0) {
				foundNumber = true;
			} else {
				current=current->next;
			}
			counter++;
		}

		if (foundNumber) {
			return(counter);
		} else {
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
	Sms sms;
	List customerList;
#endif

extern Sms sms;
extern List customerList;
