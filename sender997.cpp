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
using namespace std;

int main() {
     int qid = msgget(ftok(".",'u'), 0);

     struct buf {
		long mtype;
		char greeting[50];
	};
	
     buf msg;
	int size = sizeof(msg)-sizeof(long);

     // (1)
     strcpy(msg.greeting, "Hello first receiever");	
	cout << getpid() << ": sends message to first receiver" << endl;
	msg.mtype = 117; 
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     // (1) acknowledgement 
     msgrcv(qid, (struct msgbuf *)&msg, size, 110, 0);
	cout << getpid() << ": " << msg.greeting << endl;

     // (2)
     strcpy(msg.greeting, "Hello second receiver");	
	cout << getpid() << ": sends message to second receiver" << endl;
	msg.mtype = 217; 	
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     // (2) acknowledgement
     msgrcv(qid, (struct msgbuf *)&msg, size, 210, 0);
	cout << getpid() << ": " << msg.greeting << endl;

     // deletes message queue
     // (make sure queue is empty!)
	msgctl(qid, IPC_RMID, NULL);

     exit(0);
}