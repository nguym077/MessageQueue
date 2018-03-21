/*

Marian Nguyen
Richard Salmeron
Project 1
Files: receiver1.cpp, receiver2.cpp, sender997.cpp,
       sender251.cpp, sender257.cpp, patch64.o, get_info.h

Description: Second Receiver
- accepts messages from 257 and 997 only
- terminates after it has receieved a total of 5000 messages

*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

int main() {
	int msgCount = 0;
    int qid = msgget(ftok(".",'u'), 0);

    struct buf {
		long mtype;
		char greeting[50];
	};

    buf msg;
	int size = sizeof(msg)-sizeof(long);

	cout << "RECEIVER 2: " << getpid() << endl;

	while (msgCount < 5000) {
		// receives message from 257 or 997
		msgrcv(qid, (struct msgbuf *)&msg, size, 200, 0);
		msgCount++;
		cout << getpid() << " (Receiver2): " << msg.greeting << endl;

		if (msg.greeting[0] == '9') {
			// acknowledgement to 997
			msg.mtype = 210;
			strcpy(msg.greeting, "Message delivered to second receiver successfuly.");
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		} else {
			// dummy message to 257
			msg.mtype = 410;
			strcpy(msg.greeting, "Dummy Mesage to Sender 257");
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		}
	}

	// sends sender 997 last message
	msg.mtype = 210;
	strcpy(msg.greeting, "Terminated (Receiver 2)");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// sends sender 257 last message
	msg.mtype = 410;
	strcpy(msg.greeting, "Terminated (Receiver 2)");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// sends sender 997 terminating message (11)
	msg.mtype = 701;
	strcpy(msg.greeting, "Terminated (Receiver 2)");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// receiver 2 terminates
	cout << getpid() << ": now exits" << endl;
     exit(0);
}
