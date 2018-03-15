// Sender 257
//   - Does not accept any acknowledgement message
//   - only notifies one reciever (receiver2)
//   - terminates when its reciever stops receiving event notification

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
	
	srand(time(NULL));
	
    int qid = msgget(ftok(".",'u'), 0);

    struct buf {
		long mtype;
		char greeting[50];
	};
	
    buf msg;
	int size = sizeof(msg)-sizeof(long);

	cout << "SENDER 257" << endl;

	unsigned long randMax = 4294967295;
	unsigned long randomNumber;
	bool status257 = true;

	while (status257){
		
		randomNumber = rand()%randMax;
		// retrieves terminating message if there is one
		msgrcv(qid, (struct msgbuf *)&msg, size, 259, 0);

		if (msg.greeting[0] == 'T') {
			status257 = false;
		} 
		else if(randomNumer%257 == 0) {
			strcpy(msg.greeting, "Hello second receiever from sender257.");
			cout << getpid() << ": sends message to second receiver" << endl;
			msg.mtype = 257; 
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		}
	}
	
	//Sender 257 Terminates
	cout << getpid() << ": now exits" << endl;
     exit(0);
}
