/*

Marian Nguyen
Richard Salmeron
Project 1
Files: receiver1.cpp, receiver2.cpp, sender997.cpp,
       sender251.cpp, sender257.cpp, patch64.o, get_info.h

Description: First Receiver
- accepts messages from 251 and 997 senders only
- Terminates when both senders are terminated

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
     int qid = msgget(ftok(".",'u'), 0);

     struct buf {
          long mtype;
          char greeting[50];
     };

     buf msg;
     int size = sizeof(msg) - sizeof(long);

     cout << "RECEIVER 1: " << getpid() << endl;

     bool bothSending = true;
     int count = 0;

     while (bothSending) {
         // receives message from 251 or 997
         msgrcv(qid, (struct msgbuf *)&msg, size, 100, 0);
         cout << getpid() << " (receiver1): " << msg.greeting << endl;

         if (msg.greeting[0] == '9') {
             // message came from 997, need to send acknowledgement
             msg.mtype = 110;
             strcpy(msg.greeting, "Message delivered to first receiver successfuly.");
             msgsnd(qid, (struct msgbuf *)&msg, size, 0);
         } else if (msg.greeting[0] == 'T') {
            // terminating message received from either 251 or 997
            // if both are received, receiver1 terminates
             count++;

             if (count == 2) {
                 bothSending = false;
             }
         }
     }

    // sends sender 997 last message
    msg.mtype = 110;
  	strcpy(msg.greeting, "Terminated (Receiver 1)");
  	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     // sends sender 997 terminating message (10)
 	msg.mtype = 700;
 	strcpy(msg.greeting, "Terminated (Receiver 1)");
 	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     // receiver 1 terminates
	 cout << getpid() << ": now exits" << endl;
     exit(0);
}
