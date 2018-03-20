// Sender 997
//   - sends each event to all receivers (receiver1 and receiver2)
//   - requires acknowledgement for each message from
//        both receivers before continues execution
//   - terminates when it gets/observes a random number < 100

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
	// initialize random
	srand(time(NULL));

	// initialize message queue
	int qid = msgget(ftok(".", 'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // msg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

	srand(time(NULL));
	int randomEvent;

	cout << "SENDER 997: " << getpid() << endl;

	bool selfTerminate = false;		// used for terminating condition of < 100
	bool statusReceiver2 = true;

	// grabs random value to enter the loop
	do {
		randomEvent  = INT_MAX * rand();
	} while (randomEvent % 997 != 0 || randomEvent < 100);

	while (randomEvent >= 100) {
		// (1) -- sends message to receiver1
		msg.mtype = 100;
		strcpy(msg.greeting, "997 to first receiver. Value: " + std::to_string(randomEvent));
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		cout << getpid() << " (sender997): send message to first receiver." << endl;

		// (1) acknowledgement
		msgrcv(qid, (struct msgbuf *)&msg, size, 110, 0);
		cout << getpid() << ": " << msg.greeting << endl;

		if (statusReceiver2) {
			// (2)
			msg.mtype = 200;
			strcpy(msg.greeting, "997 to second receiver.");
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << getpid() << ": send message to second receiver." << endl;

			// (2) acknowledgement
			msgrcv(qid, (struct msgbuf *)&msg, size, 210, 0);
			cout << getpid() << ": " << msg.greeting << endl;

			if (msg.greeting[0] == 'T') {
				// receiver2 was terminated
				statusReceiver2 = false;
			}
		}

		// generates new random value
		do {
			randomEvent  = INT_MAX * rand();
		} while (randomEvent % 997 != 0 || randomEvent < 100);
	}

	cout << "Sender997 Terminated" << endl;

	// sends reciever1 last message
	msg.mtype = 100;
	strcpy(msg.greeting, "Terminated (receiver 1)");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// cannot delete/terminate queue unless all others are terminated
	// waits for terminating message from each channel
	msgrcv(qid, (struct msgbuf *)&msg, size, 700, 0);		// (10)
	msgrcv(qid, (struct msgbuf *)&msg, size, 701, 0);		// (11)
	msgrcv(qid, (struct msgbuf *)&msg, size, 702, 0);		// (12)
	msgrcv(qid, (struct msgbuf *)&msg, size, 703, 0);		// (13)

     // deletes message queue
     // (make sure queue is empty!)
	msgctl(qid, IPC_RMID, NULL);

	//Sender 997 Terminates
	cout << getpid() << ": now exits" << endl;
	exit(0);

}
