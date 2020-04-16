#ifndef _MQRequestChannel_H_
#define _MQRequestChannel_H_

#include "common.h"
#include "RequestChannel.h"


class MQRequestChannel : public RequestChannel
{

private:
	/*  The current implementation uses named pipes. */
	
	
	
	
	int wfd;
	int rfd;
	
	string pipe1, pipe2;

	string msq1, msq2; 
	int open_pipe(string _pipe_name, int mode);

	//private attributes of a msg queue///
	int num_messages =2; 
	mqd_t wmq; 
	mqd_t rmq; 

	struct mq_attr attr; 

	mqd_t open_msg_queue(string _msq_name, int mode); 
	
	
public:
	MQRequestChannel(const string _name, const Side _side, int b_size =256);
	/* Creates a "local copy" of the channel specified by the given name. 
	 If the channel does not exist, the associated IPC mechanisms are 
	 created. If the channel exists already, this object is associated with the channel.
	 The channel has two ends, which are conveniently called "SERVER_SIDE" and "CLIENT_SIDE".
	 If two processes connect through a channel, one has to connect on the server side 
	 and the other on the client side. Otherwise the results are unpredictable.

	 NOTE: If the creation of the request channel fails (typically happens when too many
	 request channels are being created) and error message is displayed, and the program
	 unceremoniously exits.

	 NOTE: It is easy to open too many request channels in parallel. Most systems
	 limit the number of open files per process.
	*/

	~MQRequestChannel();
	/* Destructor of the local copy of the bus. By default, the Server Side deletes any IPC 
	 mechanisms associated with the channel. */

	char* cread(int *len=NULL);
	/* Blocking read of data from the channel. Returns a string of characters
	 read from the channel. Returns NULL if read failed. */

	int cwrite(char *msg, int msglen);
	/* Write the data to the channel. The function returns the number of characters written
	 to the channel. */
	 
	string name(); 
};

#endif