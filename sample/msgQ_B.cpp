/* 

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program B creates a message queue to be shared with Program A.
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by Program B.

Both child processes use message type mtype = 113 and 114.

-- NOTE: "IPC_NOWAIT" is to be avoided (no halt option)

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

	// "msgget" : allocate or locate an existing queue
	// "CREAT" : allocate the queue
	// "EXCL" : guareentees the queue being created is brand new
	// "0600" : access permission (last three digits represents Read Write Execute bits)
	//			0 : owner of program
	//			6 : 1 (R) 1 (W) 0 (E)
	//			0 : group (do not have access to queue)
	//			0 : others
	// "ftok" : internal OS identifer (generates an ID for the system to use based on
	//			two parameters in ftok--path to program and user-selected integer)
	// "qid" : ID used within this program
	//
	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// holds the message that will be sent (message content)
	//
	// declare my message buffer
	struct buf {
		long mtype; // required & has to be first
		char greeting[50]; // mesg content
	};

	// "size" : so the OS knows how much memory to allocate
	buf msg;
	int size = sizeof(msg)-sizeof(long);

	// "msgrcv" : sends message to B and removes message from msgQ
	// "size" : number of bytes to copy
	// "117" : mtype
	// "0" : tells us how function will behave (here it means synchronous,
	//		in plain english -> wait till message is available ~ "cin")
	msgrcv(qid, (struct msgbuf *)&msg, size, 117, 0); // read mesg
						// don't read "fake" mesg
	cout << getpid() << ": gets message" << endl;
	cout << "message: " << msg.greeting << endl;
	
	// "struct msgbuf*" : typecast so we can fool compiler (might not need)
	// "mtype" : used to differentiate between messages. hardest part will be
	// 			deciding what mtype value to use (be very careful!)
	strcat(msg.greeting, " and Adios.");
	cout << getpid() << ": sends reply" << endl;
	msg.mtype = 314; // only reading mesg with type mtype = 314
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	cout << getpid() << ": now exits" << endl;

	// within a given mtype -> FIFO (greetings with same mtype)
	msgrcv (qid, (struct msgbuf *)&msg, size, -112, 0);
	msgrcv (qid, (struct msgbuf *)&msg, size, 0, 0);
	msgrcv (qid, (struct msgbuf *)&msg, size, 117, 0);

	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}


