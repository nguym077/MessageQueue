// Second Receiver
//   - accepts messages from 257 and 997 only
//   - terminates after it has receieved a total of 5000 messages

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
     int qid = msgget(ftok(".", 'u'), IPC_EXCL|IPC_CREAT|0600);

     struct buf {
		long mtype;
		char greeting[50];
	};
	
     buf msg;
	int size = sizeof(msg)-sizeof(long);

	cout << "RECEIVER 2" << endl;

	bool 997Status = true;
	//bool 257Status = true;
	while (msgCount < 5000) {
		if (997Status) {
			// (2)
			msgrcv(qid, (struct msgbuf *)&msg, size, 200, 0);
			cout << getpid() << ": Message receieved from sender 997" << endl;
			cout << "message: " << msg.greeting << endl;
			msgCount++;

			if (msg.greeting[0] == 'T') {
				997Status = false;
			} else {
				msg.mtype = 210;
				strcpy(msg.greeting, "Message delivered to second receiver successfuly.");
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		}

		// (4)
		msgrcv(qid, (struct msgbuf *)&msg, size, 400, 0);
		cout << getpid() << ": Message received from sender 257" << endl;
		cout << "message: " << msg.greeting << endl;
		msgCount++;
	}

	// sends sender 257 last message
	msg.mtype = 400;
	strcpy(msg.greeting, "Terminated (Receiver 2)");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// receiver 2 terminates
	cout << getpid() << ": now exits" << endl;
     exit(0);
}