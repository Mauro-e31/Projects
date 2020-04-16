#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "FIFOreqchannel.h"
#include "MQRequestChannel.h"
#include "SHMRequestChannel.h"
using namespace std;


int nchannels = 0;
pthread_mutex_t newchannel_lock;
void *handle_process_loop(void *_channel);
char ival;
vector<string> all_data [NUM_PERSONS];

string channel_type= "f";

void process_newchannel_request (RequestChannel *_channel)
{
	nchannels++;
	string new_channel_name = "data" + to_string(nchannels) + "_";
	char buf [30];
	strcpy (buf, new_channel_name.c_str());
	_channel->cwrite(buf, new_channel_name.size()+1);

	//RequestChannel *data_channel = new FIFORequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);
	//RequestChannel *data_channel = new MQRequestChannel(new_channel_name,RequestChannel::SERVER_SIDE);
	RequestChannel *data_channel;

	if(channel_type=="f"){
        cout<<"using a fifo "; 
        data_channel = new FIFORequestChannel("control",RequestChannel::SERVER_SIDE); 
    }
    else if(channel_type=="q"){
        cout<<"using a queue "<<endl; 
        data_channel = new MQRequestChannel("control",RequestChannel::SERVER_SIDE);
    }
    else if(channel_type=="s"){
        cout<<"using a shared memory"<<endl; 
        data_channel = new SHMRequestChannel("control",SHMRequestChannel::SERVER_SIDE); 
    }
	 //= new SHMRequestChannel(new_channel_name,RequestChannel::SERVER_SIDE); 
	pthread_t thread_id;
	if (pthread_create(&thread_id, NULL, handle_process_loop, data_channel) < 0){
		EXITONERROR("");
	}
}

void populate_file_data (int person){
	cout << "populating for person " << person << endl;
	string filename = "BIMDC/" + to_string(person) + ".csv";
	char line[100];
	ifstream ifs (filename.c_str());
	int count = 0;
	while (!ifs.eof()){
		line[0] = 0;
		ifs.getline(line, 100);
		if (ifs.eof())
			break;
		double seconds = stod (split (string(line), ',')[0]);
		if (line [0])
			all_data [person-1].push_back(string(line));
	}
}

double get_data_from_memory (int person, double seconds, int ecgno){
	int index = (int)round (seconds / 0.004);
	string line = all_data [person-1][index]; 
	vector<string> parts = split (line, ',');
	double sec = stod(parts [0]);
	double ecg1 = stod (parts [1]);
	double ecg2 = stod (parts [2]); 
	if (ecgno == 1)
		return ecg1;
	else
		return ecg2;
}

void process_file_request (RequestChannel* rc, char* request){
	
	filemsg * f = (filemsg *) request;
	string filename = request + sizeof (filemsg);
	filename = "BIMDC/" + filename; // adding the path prefix to the requested file name
	cout << "Server received request for file " << filename << endl;

	if (f->offset == 0 && f->length == 0){ // means that the client is asking for file size
		__int64_t fs = get_file_size (filename);
		rc->cwrite ((char *)&fs, sizeof (__int64_t));
		return;
	}
	
	// make sure that client is not requesting too big a chunk
	assert (f->length <= MAX_MESSAGE);

	
	char buffer [MAX_MESSAGE];
	FILE* fp = fopen (filename.c_str(), "rb");
	if (!fp){
		EXITONERROR ("Cannot open " + filename);
	}
	fseek (fp, f->offset, SEEK_SET);
	int nbytes = fread (buffer, 1, f->length, fp);
	assert (nbytes == f->length);
	rc->cwrite (buffer, nbytes);
	fclose (fp);
}

void process_data_request (RequestChannel* rc, char* request){
	//cout<<"process data Rquest"<<endl; 
	datamsg* d = (datamsg* ) request;
	double data = get_data_from_memory (d->person, d->seconds, d->ecgno);
	cout<<data<< " "<<d->person<< " "<<d->seconds<<" "<<d->ecgno<<endl; 
	rc->cwrite((char *) &data, sizeof (double));
}

void process_unknown_request(RequestChannel *rc){
	char a = 0;
	rc->cwrite (&a, sizeof (a));
}


int process_request(RequestChannel *rc, char* _request)
{
	
	MESSAGE_TYPE m = *(MESSAGE_TYPE *) _request;
	if (m == DATA_MSG){
		usleep (rand () % 5000);
		process_data_request (rc, _request);
	}
	else if (m == FILE_MSG){
		process_file_request (rc, _request);
			
	}else if (m == NEWCHANNEL_MSG){
		process_newchannel_request(rc);
	}
	else{
		process_unknown_request(rc);
	}
}

void *handle_process_loop(void *_channel)
{
	RequestChannel *channel = (RequestChannel *)_channel;
	for (;;){
		//cout<<"**\n\n";
		int len = 0;
		char * buffer = channel->cread(&len);
		if (len == 0)
			break;
		MESSAGE_TYPE m = *(MESSAGE_TYPE *) buffer;
		if (m == QUIT_MSG)
			break;
		process_request(channel, buffer);
		delete [] buffer;
	}
}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/


int main(int argc, char *argv[])
{
	
	

	int buffer_size; 
	int n =0; 
	int b=256;

	for(int i =0; i<argc; i++){
       // cout<<argv[i]<<endl; 
        string flag = argv[i]; 
        if(flag=="-n"){
            string s_n = argv[i+1]; 
            n = stoi(s_n);
            
        }
 
        else if(flag=="-b"){
            string s_b = argv[i+1]; 
            b = stoi(s_b); 
            
        }

        else if(flag=="-i"){
            channel_type= argv[i+1]; 
        }
       
    }


	srand(time_t(NULL));
	
	for (int i=0; i<NUM_PERSONS; i++){
		populate_file_data(i+1);
	}

	RequestChannel * control_channel; 
	//control_channel = new MQRequestChannel("control",RequestChannel::SERVER_SIDE); 
	
	if(channel_type=="f"){
        cout<<"using a fifo "; 
        control_channel = new FIFORequestChannel("control",RequestChannel::SERVER_SIDE,b); 
    }
    else if(channel_type=="q"){
        cout<<"using a queue "<<endl; 
        control_channel = new MQRequestChannel("control",RequestChannel::SERVER_SIDE,b);
    }
    else if(channel_type=="s"){
        cout<<"using a shared memory"<<endl; 
        control_channel = new SHMRequestChannel("control",SHMRequestChannel::SERVER_SIDE,b); 
    }
	
	//control_channel = new FIFORequestChannel("control",RequestChannel::SERVER_SIDE); 
	//cout<<"channel created "<<endl;
	handle_process_loop(control_channel);
	

}
