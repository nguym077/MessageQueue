/*

Sender 251
- does not accept any acknowledgement message
- Only reports its event to one receiver (receiver1)
- terminates on a kill command (patch)

The compilation would use the command:
'g++ yoursender251.cpp patchxx.o'
The 2 files will compile, link & "fuse" to produce a
single executable. Then, execute your program as usual.

kill -10 <pid>

*/

// The sender 251 source code needs to include get_info.h file
// and calls the function get_info exactly once (not in a loop),
// anywhere after the msgget function in sender 251 code.

#include "get_info.h"
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
#include <limits.h>
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

	// last message to r1 through patch
    msg.mtype = 100;
	strcpy(msg.greeting, "Terminate (sender 251 to receiver1)");
	get_info(qid, (struct msgbuf *)&msg, size, 300);

	srand(time(NULL));
	int randomEvent;

	cout << "SENDER 251: " << getpid() << endl;
	while (true) {
		// grabs value of random event for message
		do {
			randomEvent = INT_MAX * rand();
		} while (randomEvent % 251 != 0);

		// (3)
          msg.mtype = 100;
		strcpy(msg.greeting, "251 to first receiver. Value: " + std::to_string(randomEvent));
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
          cout << getpid() << " (sender251): send message to first receiver." << endl;
	}

    // sends sender 997 terminating message (12)
	msg.mtype = 702;
	strcpy(msg.greeting, "Terminated (Receiver 2)");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	//Sender 251 Terminates
	cout << getpid() << ": now exits" << endl;
	exit(0);
}
