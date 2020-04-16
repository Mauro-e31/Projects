
#ifndef _SHMRequestChannel_H_
#define _SHMRequestChannel_H_

#include "common.h"
#include "RequestChannel.h"
#include "KernelSemaphore.h"

class SHMRequestChannel : public RequestChannel
{

private:
	/*  The current implementation uses named pipes. */
	
	
	string semap1_name, semap2_name;

	KernelSemaphore *semap_read; 
	KernelSemaphore *semap_write;
	
	int wfd;
	int rfd;

	char * mem_ptr_read; 
	char * mem_ptr_write; 



	
	
	string pipe1, pipe2;

    string sm1, sm2; 

	int open_pipe(string _pipe_name, int mode);
    int open_shared_mem(string _shared_Mem_name, int mode); 

	
public:
	SHMRequestChannel(const string _name, const Side _side, int b_size =256);
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

	~SHMRequestChannel();
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
