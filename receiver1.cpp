// First Receiver
//   - accepts messages from 251 and 997 senders only

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
     int size = sizeof(msg) - sizeof(long);

     cout << "RECEIVER 1" << endl;

     // (1)
     msgrcv(qid, (struct msgbuf *)&msg, size, 100, 0);
	cout << getpid() << ": Message received from sender 997." << endl;
	cout << "message: " << msg.greeting << endl;

     msg.mtype = 110;
	strcpy(msg.greeting, "Message delivered to first receiver successfuly.");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     // (3)
     msgrcv(qid, (struct msgbuf *)&msg, size, 300, 0);
	cout << getpid() << ": Message received from sender 251." << endl;
	cout << "message: " << msg.greeting << endl;

     // receiver 1 terminates
	cout << getpid() << ": now exits" << endl;
     exit(0);
}