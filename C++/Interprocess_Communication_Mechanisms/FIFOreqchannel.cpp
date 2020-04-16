#include "common.h"
#include "FIFOreqchannel.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

FIFORequestChannel::FIFORequestChannel(const string _name, const Side _side,int b_size):
RequestChannel(_name,_side,b_size)
{
	pipe1 = "fifo_" + my_name + "1";
	pipe2 = "fifo_" + my_name + "2";
		
	if (_side == SERVER_SIDE){
		wfd = open_pipe(pipe1, O_WRONLY);
		rfd = open_pipe(pipe2, O_RDONLY);
	}
	else{
		rfd = open_pipe(pipe1, O_RDONLY);
		wfd = open_pipe(pipe2, O_WRONLY);
		
	}
	
}

FIFORequestChannel::~FIFORequestChannel()
{ 
	
	close(wfd);
	close(rfd);

	remove(pipe1.c_str());
	remove(pipe2.c_str());
	
}

int FIFORequestChannel::open_pipe(string _pipe_name, int mode)
{
	mkfifo (_pipe_name.c_str (), 0600);
	int fd = open(_pipe_name.c_str(), mode);
	if (fd < 0){
		EXITONERROR(_pipe_name);
	}
	return fd;
}

char* FIFORequestChannel::cread(int *len)
{
	char * buf = new char [max_buffer_size];
	int length; 
	length = read(rfd, buf, max_buffer_size);
	if (len)	// the caller wants to know the length
		*len = length;
	return buf;
}

int FIFORequestChannel::cwrite(char* msg, int len)
{
	if (len > max_buffer_size){
		EXITONERROR("cwrite");
	}
	if (write(wfd, msg, len) < 0){
		EXITONERROR("cwrite");
	}
	return len;
}

