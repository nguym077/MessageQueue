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
#include <cstdlib>
using namespace std;

int main() {
	// finds existing queue
     int qid = msgget(ftok(".",'u'), 0);

	// declare message buffer
     struct buf {
		long mtype;
		char greeting[50];
	};
	
     buf msg;
	int size = sizeof(msg)-sizeof(long);

	cout << "SENDER 251" << endl;

	// (3)
     strcpy(msg.greeting, "Hello first receiever from sender 251.");
	cout << getpid() << ": sends message to first receiver" << endl;
	msg.mtype = 300; 
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

     //Sender 251 Terminates
	cout << getpid() << ": now exits" << endl;
	exit(0);
}
