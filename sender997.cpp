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

	// finds existing queue
	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

	srand(time(NULL));
	int randomEvent;

	do {
		randomEvent  = INT_MAX * rand();
	} while(randomEvent % 997 != 0 || randomEvent < 100);

	cout << "SENDER 997" << endl;
	while (randomEvent >= 100) {
		// (1)
		strcpy(msg.greeting, "Hello first receiever from sender 997.");
		cout << getpid() << ": sends message to first receiver" << endl;
		msg.mtype = 100; 
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);

		// (1) acknowledgement 
		msgrcv(qid, (struct msgbuf *)&msg, size, 110, 0);
		cout << getpid() << ": " << msg.greeting << endl;

		// (2)
		strcpy(msg.greeting, "Hello second receiver from sender 997.");	
		cout << getpid() << ": sends message to second receiver" << endl;
		msg.mtype = 200; 	
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);

		// (2) acknowledgement
		msgrcv(qid, (struct msgbuf *)&msg, size, 210, 0);
		cout << getpid() << ": " << msg.greeting << endl;

		do {
			randomEvent  = INT_MAX * rand();
		} while(randomEvent % 997 != 0 || randomEvent < 100);
	}

	// sends last message to r1
	strcpy(msg.greeting, "Terminate (sender 997 to receiver1)");
	msg.mtype = 100; 
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// sends last message to r2
	strcpy(msg.greeting, "Terminate (sender 997 to receiver2)");
	msg.mtype = 200; 
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     // deletes message queue
     // (make sure queue is empty!)
	msgctl(qid, IPC_RMID, NULL);

	//Sender 997 Terminates
	cout << getpid() << ": now exits" << endl;
	exit(0);

}
