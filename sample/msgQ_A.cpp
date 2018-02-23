/* 

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program A will use a message queue created by Program B. 
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by anyone.

Both child processes use message type mtype = 113 and 114.

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

	// pause Program A
	sleep(3); 	// BAD programming - unreliable and potential bug
				// do NOT use

	// "msgget" : not allocating, just need to find an existing queue (0)
	// 			assume queue exists! make sure other program starts first
	// "ftok" : needs to be same identifier as in other program 
	//			(parameters need to match for A & B, also same directory)
	int qid = msgget(ftok(".",'u'), 0);

	// "struct" needs to be identical to the other file (B)
	// (msg objects in the queue are byte aligned)
	//
	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

	// "msgsend" : program is copying messages into queue
	// "0" : halt if queue is too full to hold this message
	//		(if done correctly, it should never stop here!)
	// 
	// sending garbage
	msg.mtype = 111;
	strcpy(msg.greeting, "Fake message");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	strcpy(msg.greeting, "Another fake");
	msg.mtype = 113;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// "gitpid()" : pid of the A program (pid is program id assigned by system)
	//
	// prepare my message to send
	strcpy(msg.greeting, "Hello there");	
	cout << getpid() << ": sends greeting" << endl;
	msg.mtype = 117; 	// set message type mtype = 117
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending

	// "msgrcv" : "314" message needed from msgQ
	msgrcv(qid, (struct msgbuf *)&msg, size, 314, 0); // reading
	cout << getpid() << ": gets reply" << endl;
	cout << "reply: " << msg.greeting << endl;
	cout << getpid() << ": now exits" << endl;

	// "msgsnd" : resusing mtype is okay
	// serves purpose -> exit protocol
	msg.mtype = 117;
	msgsnd (qid, (struct msgbuf *)&msg, size, 0);

	exit(0);
}


