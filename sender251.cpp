/*

Sender 251
- does not accept any acknowledgement message
- Only reports its event to one receiver (receiver1)
- terminates on a kill command (patch)

The compilation would use the command:
'g++ yoursender251.cpp patchxx.o'
The 2 files will compile, link & "fuse" to produce a
single executable. Then, execute your program as usual.

*/

// The sender 251 source code needs to include get_info.h file
// and calls the function get_info exactly once (not in a loop),
// anywhere after the msgget function in sender 251 code. 
// #include "get_info.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <cstdlib>
using namespace std;


// "struct" needs to be identical to the other file (B)
// (msg objects in the queue are byte aligned)
//
// declare my message buffer
struct buf {
	long mtype; // required
	char greeting[50]; // mesg content
};

int main() {
	
	// "msgget" : not allocating, just need to find an existing queue (0)
	// 			assume queue exists! make sure other program starts first
	// "ftok" : needs to be same identifier as in other program 
	//			(parameters need to match for A & B, also same directory)
	int qid = -1;
	char tempMsg[50];
	buf msg;
	int size = sizeof(msg)-sizeof(long);

	//Random 32 bit Int Generator
	random_device randGen;
	long randEvent = -1;

	while(msgget(ftok(".",'u'), 0)>=0){

		qid = msgget(ftok(".",'u'), 0);

		
	
		//Generate Randome Value
		randEvent = randGen();

		//Set message type mtype = 251
		msg.mtype = 251;

		//Check For Qualifying Random Event
		if(randEvent % msg.mtype == 0){

			// "gitpid()" : pid of the A program (pid is program id assigned by system)
			// prepare my message to send
			strcpy(msg.greeting, randEvent);
			strcpy(tempMsg, randEvent);

			cout << getpid() << ": sends greeting" << endl;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending

		}
		
	}
	

	//Sender 251 Terminates
	cout << getpid() << ": now exits" << endl;
	exit(0);
}
