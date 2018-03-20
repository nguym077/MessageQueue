/*

Marian Nguyen
Richard Salmeron
Project 1
Files: receiver1.cpp, receiver2.cpp, sender997.cpp,
       sender251.cpp, sender257.cpp, patch64.o, get_info.h

Description: Sender 257
- Does not accept any acknowledgement message
- only notifies one reciever (receiver2)
- terminates when its reciever stops receiving event notification
*/

#include <cstdio>
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
    // gets already existing message queue
     int qid = msgget(ftok(".",'u'), 0);

     struct buf {
		long mtype;
		char greeting[50];
	};

    buf msg;
	int size = sizeof(msg)-sizeof(long);

	srand(time(NULL));
	int randomEvent;

	cout << "SENDER 257: " << getpid() << endl;

	bool statusReceiver2 = true;
	while (statusReceiver2) {
		// grabs value of random event for message
		do {
			randomEvent = INT_MAX * rand();
		} while (randomEvent % 257 != 0);

		char eventString[9];
		sprintf(eventString, "%d", randomEvent);

        // (4)
        strcpy(msg.greeting, "257 to second receiver. Value: " + std::to_string(randomEvent));
		strcat(msg.greeting, eventString);
        cout << getpid() << " (sender257): sends message to second receiver" << endl;
        msg.mtype = 200;
        msgsnd(qid, (struct msgbuf *)&msg, size, 0);

		// retrieves terminating message if there is one
		msgrcv(qid, (struct msgbuf *)&msg, size, 410, 0);

		if (msg.greeting[0] == 'T') {
			statusReceiver2 = false;
		}
	}

    // sends sender 997 terminating message (13)
	msg.mtype = 703;
	strcpy(msg.greeting, "Terminated (Sender 257)");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	//Sender 257 Terminates
	cout << getpid() << ": now exits" << endl;
    exit(0);
}
