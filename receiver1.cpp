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

     // (1)
     msgrcv(qid, (struct msgbuf *)&msg, size, 117, 0);
	cout << getpid() << ": Message received from sender 997 (1)" << endl;
	cout << "message: " << msg.greeting << endl;

     msg.mtype = 110;
	strcpy(msg.greeting, "Message sent to first receiver successfuly.");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     exit(0);
}