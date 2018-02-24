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
#include <cstdlib>
using namespace std;

int main() {
     int qid = msgget(ftok(".",'u'), 0);

     struct buf {
		long mtype;
		char greeting[50];
	};
	
     buf msg;
	int size = sizeof(msg)-sizeof(long);





     exit(0);
}