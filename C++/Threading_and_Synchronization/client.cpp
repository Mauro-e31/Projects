#include "common.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
#include "common.h"
#include "HistogramCollection.h"
#include "FIFOreqchannel.h"
#include <fstream>
using namespace std;


/* ******************************OLD CLIENT FUNCIONS****************** */
int copy_file_from_server(string fileName,string file_path,FIFORequestChannel & chan,string file_type1="\0"); 
//int copy_file_from_server_type(string fileName,string file_path,FIFORequestChannel & chan,string file_type="y.csv");
int data_points_files_(int number_files,string file_path,FIFORequestChannel& chan); 
double patient_data_points(int patient,double time, int ecg,string file_path,FIFORequestChannel& chan ); 
string get_new_channel(FIFORequestChannel & chan); 
int shut_channel(FIFORequestChannel& chan2); 

/////////////************NEW FUNCTIONS*****************/////////////////////

class patient_thread_args
{
  public:
    /*put all the arguments for the patient threads */
    int patient = 0;
    double time = 0;
    int ecg = 1;
    BoundedBuffer *shared_buffer;
    int request_ammount;
    int remaining_bytes; 
    string file_name; 
    int max_buffer_m =256; 
    //Histogram * personal_h ;
};

class worker_thread_args
{
  public:
    /*put all the arguments for the wroker threads */
    int number_of_request = 0;
    BoundedBuffer *shared_buffer;
    pthread_mutex_t *shared_mutx;
    FIFORequestChannel *personal_channel;
    vector<Histogram *> *shared_h_list;
    string file_name; 

    string channel_name;
};

////////////FUNCTIONS DEFINITIONS ////////////////////////
void *patient_thread_function(void *arg)
{
    /* What will the patient threads do? */

    patient_thread_args *parent = (patient_thread_args *)arg;
    int request_ammount = parent->request_ammount;
    double time_ = 0.0;
    //cout<<"parent id "<<parent->patient<<endl;
    for (int i = 0; i < request_ammount; i++)
    {

        datamsg patient_request(parent->patient, time_, parent->ecg);

        char *request1 = (char *)&patient_request;
        BoundedBuffer *temp_buffer = parent->shared_buffer;

        //cout<<"expecting size of request to be ->"<<sizeof(datamsg)<<endl<<endl;
        temp_buffer->push(request1, sizeof(datamsg));
        time_ = time_ + 0.004;
    }
}

//producer function for file request //
void *patient_file_thread_function(void *arg)
{
    /* What will the patient threads do? */

    patient_thread_args *parent = (patient_thread_args *)arg;
    int total_trips = parent->request_ammount;
    int remaining_bytes = parent->remaining_bytes; 
    int m = parent->max_buffer_m; 
    string file_name = parent->file_name; 
    ////////start data trips///////////////////
    ////////first: request file size ///////////
    char file_size_rc[sizeof(string)+sizeof(filemsg)+1]; 
    char* request_add = &(file_size_rc[0]); 
    char name[file_name.size()+1];
    strcpy(name,file_name.c_str() );

    /////////////preparing first message//////////////

    filemsg* first_message = (filemsg*)file_size_rc; 
    first_message->mtype =FILE_MSG; 
    strcpy(request_add+sizeof(filemsg),&(name[0]) );
    int64_t offset= 0; 
    int file_length=0; 
    int total =0; 
    cout<<"parent file name "<<file_name<<endl ;
    cout<<"parent total trips = "<<total_trips<<endl; 
    for(int i =0; i<total_trips; i++){
        if(!(i+1<total_trips) && (remaining_bytes!=0)){

            first_message->length=remaining_bytes; 
            first_message->offset=offset; 
            BoundedBuffer * temp_buffer = parent->shared_buffer; 
            temp_buffer->push(request_add,sizeof(filemsg)+sizeof(string)+1);
            //cout<<"pushing into buffer "<<endl; 
            total++; 

            //chan.cwrite(request_add,sizeof(filemsg)+sizeof(string)+1); 
            break; 
        }
        else{
            first_message->length=m; 
            first_message->offset=offset; 
            BoundedBuffer * temp_buffer = parent->shared_buffer; 
            temp_buffer->push(request_add,sizeof(filemsg)+sizeof(string)+1);
            total++; 
            //chan.cwrite(request_add,sizeof(filemsg)+sizeof(string)+1); 
            offset= offset+ m; 
        }

    }
    cout<<"total so far is ="<<total<<endl; 

}

//consumer
void *worker_file_thread_function(void *arg){
   worker_thread_args *worker_thread = (worker_thread_args *)arg;

    ///////////// getting request from buffer/////////////////////

    BoundedBuffer *temp_bounded_buffer = worker_thread->shared_buffer;
    FIFORequestChannel *chan = worker_thread->personal_channel;
    pthread_mutex_t *workers_mutx = worker_thread->shared_mutx;
    //cout<<worker_thread->channel_name<<endl;
    //pthread_mutex_t * mutx = & worker_thread->mut;
    string file_name = worker_thread->file_name; 
    fstream personal_fs; 
    //personal_fs.open( file_name.c_str(), std::ios::in | std::ios::out);

    pthread_mutex_lock(workers_mutx);
    while (temp_bounded_buffer->get_completed_request() != temp_bounded_buffer->get_total_request())
    {
        
        vector<char> request = temp_bounded_buffer->pop();
        pthread_mutex_unlock(workers_mutx); 
         
        
        
        filemsg * file_message = (filemsg * )&request[0]; 
        int offset = file_message->offset; 
        int buffer_length = file_message->length;
        //cout<<"buffer_size "<<buffer_length<<endl;  
        //cout<<"size of msg = "<<sizeof(filemsg)<<endl; 
        char * request_addr = &request[0]; 

        string file_name = request_addr + sizeof(filemsg);
        pthread_mutex_lock(workers_mutx); 
        chan->cwrite(&request[0],request.size() );
         

        int length_1 = 0;
        //cout<<"request size = "<< request.size()<<endl; 
        //cout<<"offset = "<<offset<<endl; 
        //cout<<"file name being processed = "<< file_name<<endl;
         
        char *last_buffer = chan->cread(&length_1);
        pthread_mutex_unlock(workers_mutx);
        
        //cout<<last_buffer;
        //cout<<"*********"<<endl<<endl; ;
    
         //pthread_mutex_lock(workers_mutx);
         
        
        

        
        
        personal_fs.open( file_name.c_str(), std::ios::in | std::ios::out);
        personal_fs.seekp(offset,personal_fs.beg);
        personal_fs.write(last_buffer, buffer_length);
        personal_fs.close();

 pthread_mutex_lock(workers_mutx);

        
   

        //thread_fs.write()
        
         

       
 
     
        
        //fwrite(last_buffer,1,length_1,)
        //fwrite(last_buffer, 1, , cout);

        
        
    }
    pthread_mutex_unlock(workers_mutx);
    //personal_fs.close(); 
    //pthread_mutex_unlock(& mutx );
}

void *worker_thread_function(void *arg)
{
    /*
		Functionality of the worker threads	
    */

    worker_thread_args *worker_thread = (worker_thread_args *)arg;

    ///////////// getting request from buffer/////////////////////

    BoundedBuffer *temp_bounded_buffer = worker_thread->shared_buffer;
    FIFORequestChannel *chan = worker_thread->personal_channel;
    pthread_mutex_t *workers_mutx = worker_thread->shared_mutx;


    //cout<<worker_thread->channel_name<<endl;
    //pthread_mutex_t * mutx = & worker_thread->mut;

    pthread_mutex_lock(workers_mutx);
    while (temp_bounded_buffer->get_completed_request() != temp_bounded_buffer->get_total_request())
    {

        vector<char> request = temp_bounded_buffer->pop();

        //pthread_mutex_unlock(& mutx );
        pthread_mutex_unlock(workers_mutx);
        datamsg *message_data = (datamsg *)&request[0];
        int patient_number = message_data->person;
        double time_requested = message_data->seconds;
        //HistogramCollection * p_histogram = worker_thread->shared_buffer;
        vector<Histogram *> histograms = *worker_thread->shared_h_list;

        //cout<< message_data->person<<endl;
        chan->cwrite(&request[0], request.size());
        /////////////read the data////////////
        int length = 0;
        char *test_buffer = chan->cread(&length);
        double *recieved_data1 = (double *)test_buffer;

        double data_collected = *recieved_data1;
        //cout<<histograms.size();
        pthread_mutex_lock(workers_mutx);
        histograms[patient_number - 1]->update(data_collected);

        //pthread_mutex_lock(&mutx);
        // cout<<"patient = "<<patient_number<< " time_requested = "<<time_requested<<" data= "<<data_collected<<endl<<endl;
    }
    pthread_mutex_unlock(workers_mutx);
    //pthread_mutex_unlock(& mutx );

    ////////initializing functions///////////
}

/////*********************MAIN PROGRAM*****************//////////
int main(int argc, char *argv[])
{
    int n = 150;    //default number of requests per "patient"
    int p = 15;     // number of patients [1,15]
    int w = 500;    //default number of worker threads1
    int b = 50; 	// default capacity of the request buffer, you should change this default
	int m = MAX_MESSAGE; 	// default capacity of the file buffer
    srand(time_t(NULL));
    bool data_points = true; 
    bool file_request = false;
    string file_name = "500k.dat"; 
    string m_size = "256"; 

    for(int i =0; i<argc; i++){
       // cout<<argv[i]<<endl; 
        string flag = argv[i]; 
        if(flag=="-n"){
            string s_n = argv[i+1]; 
            n = stoi(s_n);
            
        }
        else if(flag== "-p"){
            string s_p = argv[i+1];
            p = stoi(s_p); 
            

        }
        else if(flag=="-b"){
            string s_b = argv[i+1]; 
            b = stoi(s_b); 
            
        }
        else if(flag =="-w"){
            string s_w = argv[i+1]; 
            w= stoi(s_w); 
        }
        else if(flag=="-f"){
            file_name= argv[i+1]; 
            file_request= true; 
            data_points=false; 
            p=1;
        }
        else if(flag=="-m"){
            string s_m = argv[i+1]; 
            m_size= s_m; 
            m = stoi(s_m); 
        }
    }

    int pid = fork();
    vector<string> cmd_line; 
    cmd_line.push_back("./dataserver"); 
    cmd_line.push_back("-m"); 
    cmd_line.push_back(m_size); 
    if (pid == 0){
        const char *file_name_ = cmd_line[0].c_str();

        char *cmd_test[cmd_line.size() + 1];
        for (int i = 0; i < cmd_line.size(); i++)
        {
            char const *temp = cmd_line[i].c_str();
            cmd_test[i] = (char *)temp;
        }

        cmd_test[cmd_line.size()] = NULL;

        execvp(file_name_,cmd_test);
        // modify this to pass along m

        //execl ("dataserver", "dataserver", (char *)NULL);
        
    }
    
	FIFORequestChannel* chan = new FIFORequestChannel("control", FIFORequestChannel::CLIENT_SIDE);
    BoundedBuffer request_buffer(b);
    request_buffer.set_request_amount(n*p); 
	HistogramCollection hc;

    struct timeval start, end;
    gettimeofday(&start, 0);
    ///////////////////////////////////////////////////////

    ////////////////////////////
    //file request

   // cout << "COPYING FILE \n";
    /*struct  timeval start,end;
    gettimeofday(&start,NULL); 
    ios_base::sync_with_stdio(false); */
    ////////first: request file size ///////////
    //cout<<"m = "<<m <<endl; 
    int total_trips =0; 
    int remaining_bytes=0;
    if( file_request){
    char file_size_rc[sizeof(string) + sizeof(filemsg) + 1];
    char *request_add = &(file_size_rc[0]);
    char name[file_name.size() + 1];
    strcpy(name, file_name.c_str());

    /////////////preparing first message//////////////

    filemsg *first_message = (filemsg *)file_size_rc;
    first_message->length = 0;
    first_message->offset = 0;
    first_message->mtype = FILE_MSG;
    strcpy(request_add + sizeof(filemsg), &(name[0]));

    ///////////////sending first message //////////////

    chan->cwrite(request_add, sizeof(filemsg) + sizeof(string) + 1);

    ////////////////reading buffer //////////////
    int length_1 = 0;
    char *first_M_buffer = chan->cread(&length_1);
    int64_t *rc_file_size = (int64_t *)first_M_buffer;

    /////////////////calculating trips /////////////
    
    int64_t current_file_size = *rc_file_size;
    total_trips= (current_file_size / m);
    remaining_bytes = current_file_size % m;
    if (remaining_bytes != 0)
    {
        total_trips = total_trips + 1;
    }
    ////////////////////////////
     //cout<<"current file size = "<<current_file_size<<endl; 
    }
    //cout<<"total trips = "<<total_trips; 
   

    fstream file_stream;
    file_stream.open(file_name.c_str(), std::ios::out);
    /*for(int i =0; i< current_file_size; i++){
        file_stream.write(c,1); 
    }*/
    file_stream.close();

    /* Start all threads here */
    //**************THREADS*************//
    pthread_t p_threads_ID[p];
    pthread_t w_threads_ID[w];

    double current_time = 0.0;
    
    vector<patient_thread_args> producer_args;
    if (file_request)
    {
        patient_thread_args new_patient;
        new_patient.shared_buffer = &request_buffer;
        new_patient.request_ammount = total_trips;
        request_buffer.set_request_amount(total_trips);
        new_patient.remaining_bytes = remaining_bytes;
        new_patient.file_name = file_name;
        new_patient.max_buffer_m= m; 
        producer_args.push_back(new_patient);

        pthread_create(&p_threads_ID[0], 0, patient_file_thread_function, &producer_args[0]);
    }

    vector<worker_thread_args> worker_args;
    pthread_mutex_t worker_mutx = PTHREAD_MUTEX_INITIALIZER;
    vector<Histogram *> histogram_list;

    for (int i = 0; i < w; i++)
    {
        worker_thread_args new_worker;
        string personal_channel_name = get_new_channel(*chan);
        FIFORequestChannel *chan_w = new FIFORequestChannel(personal_channel_name, FIFORequestChannel::CLIENT_SIDE);
        //FIFORequestChannel chan_w (personal_channel_name,FIFORequestChannel::CLIENT_SIDE);
        new_worker.channel_name = personal_channel_name;
        new_worker.personal_channel = chan_w;
        new_worker.shared_buffer = &request_buffer;
        new_worker.shared_mutx = &worker_mutx;
        new_worker.shared_h_list = &histogram_list;
        new_worker.file_name = file_name;
        worker_args.push_back(new_worker);
        /* initialize channels */
    }
    if (file_request)
    {
        for (int i = 0; i < w; i++)
        {
            pthread_create(&w_threads_ID[i], 0, worker_file_thread_function, &(worker_args[i]));
        }
    }

    //vector<patient_thread_args> producer_args;
    //only if we want to request vallue points //
    if (data_points)
    {

        for (int i = 0; i < p; i++)
        {
            patient_thread_args new_patient;
            new_patient.patient = i + 1;
            //cout << new_patient.patient << endl;
            new_patient.time = current_time;
            new_patient.ecg = 1;
            new_patient.shared_buffer = &request_buffer;
            new_patient.request_ammount = n;

            // histograms
            Histogram *producer_h = new Histogram(20, -10, 10);

            hc.add(producer_h);
            histogram_list.push_back(producer_h);
            producer_args.push_back(new_patient);
        }

        /////////////////////patient threats/////////
        int expected_value = 0;
        for (int i = 0; i < p; i++)
        {

            pthread_create(&p_threads_ID[i], 0, patient_thread_function, &(producer_args[i]));
            //p_id++;
            //expected_value++;
        }
    }

    ////////////worker threads/////////
    //////////////////////////////////
    ///initiaing channels ///
    if (data_points)
    {

        for (int i = 0; i < w; i++)
        {
            pthread_create(&w_threads_ID[i], 0, worker_thread_function, &(worker_args[i]));
        }
    }

    /* Join all threads here */
    
    for (int i = 0; i < p; i++)
    {
        //cout<<"parent "<<i<<" finished  -> buff size = "<<request_buffer.get_size()<<"completed = "<<request_buffer.get_completed_request()<<endl;
        pthread_join(p_threads_ID[i], 0);
    }
    cout<<"parents finished "<<endl;

    for (int i = 0; i < w; i++)
    {
        pthread_join(w_threads_ID[i], 0);
        //cout<<"worker "<<i<<" finished "<<endl;
    }  

    cout << "size of buffer = " << request_buffer.get_size() << endl;
    for (int i = 0; i < w; i++)
    {
        FIFORequestChannel *to_be_del = worker_args[i].personal_channel;
        shut_channel(*to_be_del);
        //cout<<"here right now "<<endl;
        delete to_be_del;
  }
  gettimeofday(&end, 0);

  /////////////***************************///////////////////
  hc.print();
  int secs = (end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec) / (int)1e6;
  int usecs = (int)(end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec) % ((int)1e6);
  cout << "Took " << secs << " seconds and " << usecs << " micor seconds" << endl;

  MESSAGE_TYPE q = QUIT_MSG;
  chan->cwrite((char *)&q, sizeof(MESSAGE_TYPE));
  cout << "All Done!!!" << endl;
  delete chan;
 
    
}



















/*OLD FUNCTION DEFINITIONS */
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////*********CLIENT FUNCTION DECLARATION*************/////////////////

int copy_file_from_server(string fileName,string file_path,FIFORequestChannel & chan,string file_type1){
    cout<<"COPYING FILE \n";
    /*struct  timeval start,end;
    gettimeofday(&start,NULL); 
    ios_base::sync_with_stdio(false); */
    ////////first: request file size ///////////
    char file_size_rc[sizeof(string)+sizeof(filemsg)+1]; 
    char* request_add = &(file_size_rc[0]); 
    char name[fileName.size()+1];
    strcpy(name,fileName.c_str() );

    /////////////preparing first message//////////////

    filemsg* first_message = (filemsg*)file_size_rc; 
    first_message->length=0; 
    first_message->offset=0; 
    first_message->mtype =FILE_MSG; 
    strcpy(request_add+sizeof(filemsg),&(name[0]) ); 

    ///////////////sending first message //////////////

    chan.cwrite(request_add,sizeof(filemsg)+sizeof(string)+1); 

    ////////////////reading buffer //////////////
    int length_1 =0; 
    char* first_M_buffer = chan.cread(&length_1); 
    int64_t* rc_file_size = (int64_t*)first_M_buffer;

    /////////////////calculating trips /////////////
    int64_t current_file_size = *rc_file_size; 
    int total_trips = ( current_file_size/MAX_MESSAGE ); 
    int remaining_bytes = current_file_size%MAX_MESSAGE; 
    if(remaining_bytes!=0){
        total_trips=total_trips+1; 
    }

    //cout<<current_file_size<< "<-file size : total trips = "<<total_trips<< " remaining = "<<remaining_bytes<<'\n';
    //////////////begin opening new file  /////////
    int64_t offset= 0; 
    int file_length=0; 
    FILE * pFile; 
    string finished_file_name = file_path+fileName.substr(0,3)+file_type1; 

    pFile= fopen(finished_file_name.c_str(),"wb"); 

    ////////start data trips///////////////////

    for(int i =0; i<total_trips; i++){
        if(!(i+1<total_trips) && (remaining_bytes!=0)){

            first_message->length=remaining_bytes; 
            first_message->offset=offset; 

            chan.cwrite(request_add,sizeof(filemsg)+sizeof(string)+1); 

            length_1=0; 
            char* last_buffer = chan.cread(&length_1); 
            fwrite(last_buffer,1,remaining_bytes,pFile); 
            break; 
        }
        else{
            first_message->length=MAX_MESSAGE; 
            first_message->offset=offset; 

            chan.cwrite(request_add,sizeof(filemsg)+sizeof(string)+1); 

            length_1=0; 
            char* last_buffer = chan.cread(&length_1); 
            fwrite(last_buffer,1,MAX_MESSAGE,pFile);
            offset= offset+ MAX_MESSAGE; 
        }

    }/*
    gettimeofday(&end,NULL); 

    double time_taken= (end.tv_sec-start.tv_sec) * 1e6;
        time_taken = (time_taken + (end.tv_usec -  
                              start.tv_usec)) * 1e-6; 


       cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(6); 
        cout << " sec" << endl; */
	return 0; 
}



int data_points_files_(int number_files,string file_path,FIFORequestChannel& chan){
    cout<<"COPYING ALL DATA POINTS\n";
    /*struct  timeval start,end;
    gettimeofday(&start,NULL); 
    ios_base::sync_with_stdio(false); */


    ofstream myfile; 

    for(int i=number_files; i<=number_files; i++){
        string file_number = to_string(i); 
        myfile.open(file_path+file_number+"x.csv"); 
//NOTE: 60
        for(double j =0.0; abs(0.064-j)>0.000001; j=j+0.004){
            datamsg patient_inf(i,j,1);
            char* request_1 = (char*)&patient_inf; 
            chan.cwrite(request_1,sizeof(datamsg) ); 
            /////////////read the data////////////
            int length=0; 
            char* test_buffer = chan.cread(&length); 
            double* recieved_data1 = (double*) test_buffer;
            //////////////request_second data/////////////
            datamsg patient_info2(i,j,2); 
            char* request_2 = (char*)&patient_info2; 
            chan.cwrite(request_2,sizeof(datamsg) ); 

            //////////////////read the 2nd data/////////////////
            length=0; 
            char* test_buffer2 = chan.cread(&length); 
            double* recieved_data2= (double*) test_buffer2; 

            ///////////////////writing info into a file /////////
            myfile<<j<<","<<*recieved_data1<<","<<*recieved_data2<<"\n";
            cout<<j<<","<<*recieved_data1<<","<<*recieved_data2<<"\n";
        }
        cout<<"file = "<<i<<"....complete\n"; 
        myfile.close(); 
   /* gettimeofday(&end,NULL); 

    double time_taken= (end.tv_sec-start.tv_sec) * 1e6;
        time_taken = (time_taken + (end.tv_usec -  
                              start.tv_usec)) * 1e-6; 


       cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(6); 
        cout << " sec" << endl; */
    }



    return 0; 
}


////////////////////////////////////////////////////////////
double patient_data_points(int patient,double time, int ecg,string file_path,FIFORequestChannel& chan ){ 
     if(time<0 || time >59.996){
         cout<<"Invalid Time \n";
         return -1; 
     }
     datamsg patient_inf(patient,time,ecg);
     char* request_1 = (char*)&patient_inf; 
     chan.cwrite(request_1,sizeof(datamsg) );        
     /////////////read the data////////////       
     int length=0;        
     char* test_buffer = chan.cread(&length);        
     double* recieved_data1 = (double*) test_buffer;                         


    return (*recieved_data1);
}



string get_new_channel(FIFORequestChannel & chan){
    MESSAGE_TYPE message = NEWCHANNEL_MSG; 

    char* add_new_channel = (char*) &message; 

    chan.cwrite(add_new_channel,sizeof(MESSAGE_TYPE) ); 
    int length =0; 
    char * buffer_channel = chan.cread(&length); 

    string channel_name = buffer_channel; 

    return channel_name; 
}


int shut_channel(FIFORequestChannel& chan2){

    MESSAGE_TYPE message = QUIT_MSG; 

    char* add_new_channel = (char*) &message; 
    chan2.cwrite(add_new_channel,sizeof(MESSAGE_TYPE) );
    
    return 0; 
}