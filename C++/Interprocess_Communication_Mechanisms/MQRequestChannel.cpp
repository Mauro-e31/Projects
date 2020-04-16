#include "common.h"
#include "MQRequestChannel.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

MQRequestChannel::MQRequestChannel(const string _name, const Side _side,int b_size):
RequestChannel(_name,_side,b_size)
{

    this->attr.mq_msgsize = b_size; 
	this->attr.mq_maxmsg = 2; 

	msq1 = "/msq_" + my_name + "1"; 
	msq2 = "/msq_" + my_name + "2"; 

	if(_side ==SERVER_SIDE){
		cout<<"server side "<<endl; 
		wmq = open_msg_queue(msq1,O_WRONLY); 
		rmq = open_msg_queue(msq2,O_RDONLY); 
	}
	else{
		cout<<"client side "<<endl; 
		rmq = open_msg_queue(msq1,O_RDONLY); 
		wmq = open_msg_queue(msq2,O_WRONLY); 
	}
	
	
}

MQRequestChannel::~MQRequestChannel()
{ 
	mq_close(wmq); 
	mq_close(rmq); 

	mq_unlink(msq2.c_str() ); 
	mq_unlink(msq1.c_str() ); 

}


mqd_t MQRequestChannel::open_msg_queue(string _msq_name, int mode){
	mqd_t fd = mq_open(_msq_name.c_str(),O_CREAT | mode,0644,&attr); 
	//cout<<"channels have been opened"<<endl; 
	if(fd<0){
		perror("couln't open msg_q \n");
	}

	return fd; 
}

char* MQRequestChannel::cread(int *len)
{ /*
	char * buf = new char [MAX_MESSAGE];
	int length; 
	length = read(rfd, buf, MAX_MESSAGE);
	if (len)	// the caller wants to know the length
		*len = length;
	return buf;
	*/
/*
	cout<<"read is being opened from "; 
	if (my_side == SERVER_SIDE)
	{
		cout << "server side " ;
	}
	else
	{
		cout << "client side " ;
	}
	cout<<endl; 
	*/

	
	char * buf = new char [max_buffer_size]; 
	ssize_t recieve_data_size=-1; 
	int length=0;  
	//while(recieve_data_size < 0 ){
		//cout<<"stuck in this loop "<<endl; 
	recieve_data_size= mq_receive(rmq,buf,max_buffer_size,NULL);
	if (len)	// the caller wants to know the length
		*len = recieve_data_size; //length; 
	//}

	//cout<<"stuck here "<<my_side<<endl; 

	
	return buf; 
}

int MQRequestChannel::cwrite(char* msg, int len)
{
	/*
	cout<<"write is being opened from ";

	if (my_side == SERVER_SIDE)
	{
		cout << "server side " ;
	}
	else
	{
		cout << "client side " ;
	}
	*/
	/*
	if (len > MAX_MESSAGE){
		EXITONERROR("cwrite");
	}
	if (write(wfd, msg, len) < 0){
		EXITONERROR("cwrite");
	}
	return len;
	*/

	if (len > max_buffer_size){
		EXITONERROR("cwrite");
	}
	if (mq_send(wmq, msg, len,0) < 0){
		EXITONERROR("cwrite");
	}
	return len;
}

