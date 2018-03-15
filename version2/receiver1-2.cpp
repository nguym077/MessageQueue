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
#include <stdlib.h>
#include <time.h>
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

     bool status997 = true;
     bool status251 = true;

     while (status997 || status251) {
          if (status997) {
               // (1)
               if(msgrcv(qid, (struct msgbuf *)&msg, size, 100, 0) == 0){
	               cout << getpid() << ": Message received from sender 997." << endl;
	               cout << "message: " << msg.greeting << endl;
	
	               if (msg.greeting[0] == 'T') {
	                    status997 = false;
	               } 
				   else {
	                    msg.mtype = 110;
	                    strcpy(msg.greeting, "Message delivered to first receiver successfuly.");
	                    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	                }
               }
          }

          if (status251) {
               // (3)
               if(msgrcv(qid, (struct msgbuf *)&msg, size, 300, 0) == 0){
	               cout << getpid() << ": Message received from sender 251." << endl;
	               cout << "message: " << msg.greeting << endl;
	
	               if (msg.greeting[0] = 'T') {
	                    status251 = false;
	               }	
			   }
               
          }
     }
     
    // sends sender 997 last message
	msg.mtype = 998;
	strcpy(msg.greeting, "Terminated (Receiver 1)");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     // receiver 1 terminates
	cout << getpid() << ": now exits" << endl;
     exit(0);
}
