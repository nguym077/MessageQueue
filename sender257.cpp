// Sender 257
//   - Does not accept any acknowledgement message
//   - only notifies one reciever (receiver2)
//   - terminates when its reciever stops receiving event notification

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <limits.h>
using namespace std;

int main() {
     int qid = msgget(ftok(".",'u'), 0);

     struct buf {
		long mtype;
		char greeting[50];
	};
	
     buf msg;
	int size = sizeof(msg)-sizeof(long);

	srand(time(NULL));
	int randomEvent;

	cout << "SENDER 257" << endl;
	bool status257 = true;
	while (status257) {
		// grabs value of random event for message
		do {
			randomEvent = INT_MAX * rand();
		} while (randomEvent % 257 != 0);

		// retrieves terminating message if there is one
		msgrcv(qid, (struct msgbuf *)&msg, size, 400, 0);

		if (msg.greeting[0] == 'T') {
			status257 = false;
		} else {
			// (4)
			strcpy(msg.greeting, "Hello second receiever from sender257. Value: ");
			// strcat(msg.greeting, randomEvent);
			cout << getpid() << ": sends message to second receiver" << endl;
			msg.mtype = 400; 
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		}
	}

	//Sender 257 Terminates
	cout << getpid() << ": now exits" << endl;
     exit(0);
}
