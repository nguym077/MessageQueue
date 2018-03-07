// Sender 997
//   - sends each event to all receivers (receiver1 and receiver2)
//   - requires acknowledgement for each message from
//        both receivers before continues execution
//   - terminates when it gets/observes a random number < 100

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
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
		
		//Check if random event is less than 100
		if(randEvent < 100){
			//Sender 997 Terminates
			cout << getpid() << ": now exits" << endl;
			exit(0);
		}

		//Set message type mtype = 997
		msg.mtype = 997;

		//Check For Qualifying Random Event
		if(randEvent % msg.mtype == 0 ){

			// "gitpid()" : pid of the A program (pid is program id assigned by system)
			// prepare my message to send
			strcpy(msg.greeting, randEvent);
			strcpy(tempMsg, randEvent);

			cout << getpid() << ": sends greeting" << endl;
			
			//Set message type mtype = 251
			msg.mtype = 251;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending


			//Set message type mtype = 257
			msg.mtype = 257;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending

			// "msgrcv" : "997" 2 acknowledgement messages needed from msgQ
			int ackNum = 0;
			while(ackNum < 2){

				if(msgrcv(qid, (struct msgbuf *)&msg, size, 997, 0) == 0){ // reading

					cout << getpid() << ": gets acknowledgment << endl;
					cout << "reply: " << msg.greeting << endl;
					ackNum++;
				}
			}

			cout << getpid() << ": continues..." << endl;

		}
		
	}


	// deletes message queue
	// (make sure queue is empty!)
	msgctl(qid, IPC_RMID, NULL);

	//Sender 997 Terminates
	cout << getpid() << ": now exits" << endl;
	exit(0);

}
