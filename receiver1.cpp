// First Receiver
//   - accepts messages from 251 and 997 senders only
//   - termiantes when both senders are terminated

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

     bool 997Status = true;
     bool 251Status = true;

     while (997Status && 251Status) {
          if (997Status) {
               // (1)
               msgrcv(qid, (struct msgbuf *)&msg, size, 100, 0);
               cout << getpid() << ": Message received from sender 997." << endl;
               cout << "message: " << msg.greeting << endl;

               if (msg.greeting[0] == 'T') {
                    997Status = false;
               } else {
                    msg.mtype = 110;
                    strcpy(msg.greeting, "Message delivered to first receiver successfuly.");
                    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
               }
          }

          if (251Status) {
               // (3)
               msgrcv(qid, (struct msgbuf *)&msg, size, 300, 0);
               cout << getpid() << ": Message received from sender 251." << endl;
               cout << "message: " << msg.greeting << endl;

               if (msg.greeting[0] = 'T') {
                    251Status = false;
               }
          }
     }

     

     // receiver 1 terminates
	cout << getpid() << ": now exits" << endl;
     exit(0);
}