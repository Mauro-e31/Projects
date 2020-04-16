/*
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date  : 2/8/19
 */
#include "common.h"
#include "FIFOreqchannel.h"
#include "MQRequestChannel.h"
#include "SHMRequestChannel.h"
#include "KernelSemaphore.h"
#include <string>
#include <cstring>
#include <sys/wait.h>


/////////////**********CLIENT FUNCTIONS***********/////////////////

int copy_file_from_server(string fileName,string file_path,RequestChannel & chan,string file_type1="\0"); 
//int copy_file_from_server_type(string fileName,string file_path,FIFORequestChannel & chan,string file_type="y.csv");
int data_points_files_(int number_files,string file_path,FIFORequestChannel& chan); 
double patient_data_points(int patient,double time, int ecg,string file_path,RequestChannel& chan ); 
string get_new_channel(RequestChannel & chan); 
int shut_channel(RequestChannel& chan2); 


using namespace std;


int main(int argc, char *argv[]){
    
    int n = 1;    // default number of requests per "patient"
	int p = 15;		// number of patients
    srand(time_t(NULL));
    int w=1; 



    string channel_type = "f"; 
    
    int b= 256; 


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
        else if(flag=="-i"){
            channel_type= argv[i+1]; 
        }
       
    }

    
    ////////////////Ostream file output//////////////
    ofstream myfile;
    string file_path ="/home/mauro/Documents/csce313/csce_313_pa5/received/"; 
    string fileName= "1.csv"; 
    string binary_file = "100.dat"; 
    string binary_file2= "2.csv"; 
    string binary_file3= "500k.dat";
    string binary_file4= "512.dat"; 
    string data_server = "dataserver";
    int patients =5;  

    int pid = fork();
    vector<string> cmd_line; 
    cmd_line.push_back("./dataserver"); 
    cmd_line.push_back("-b"); 
    cmd_line.push_back("256"); 
    cmd_line.push_back("-i"); 
    cmd_line.push_back(channel_type); 
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

    else{
    

    RequestChannel * chan ; 
    if(channel_type=="f"){
        cout<<"using a fifo "; 
        chan = new FIFORequestChannel("control",RequestChannel::CLIENT_SIDE,b); 
    }
    else if(channel_type=="q"){
        cout<<"using a queue "<<endl; 
        chan = new MQRequestChannel("control",RequestChannel::CLIENT_SIDE,b);
    }
    else if(channel_type=="s"){
        cout<<"using a shared memory"<<endl; 
        chan = new SHMRequestChannel("control",SHMRequestChannel::CLIENT_SIDE,b); 
    }
    //chan = new FIFORequestChannel("control",RequestChannel::CLIENT_SIDE); 
    //chan = new MQRequestChannel("control",RequestChannel::CLIENT_SIDE); 
    //chan = new SHMRequestChannel("control",RequestChannel::CLIENT_SIDE,256); 
    cout<<"\nrequested value "<<patient_data_points(1,0,2,file_path,*chan)<<"\n";

    /////////////////copy text file ////////////////////
    //NOTE: the last argument takes the type of file
    // by defualt it has no type///
    copy_file_from_server(fileName,file_path,*chan,"y.csv" );

    ///create a new channel and return name /////////////

    string channel_name = get_new_channel(*chan); 
    cout<<"\n\n\n\n\nNEW CHANNEL NAME - > "<<channel_name<<"\n\n\n\n\n\n"; 
    RequestChannel *chan2; 
    if(channel_type=="f"){
        cout<<"using a fifo "; 
        chan2 = new FIFORequestChannel(channel_name,RequestChannel::CLIENT_SIDE,b); 
    }
    else if(channel_type=="q"){
        cout<<"using a queue "<<endl; 
        chan2 = new MQRequestChannel(channel_name,RequestChannel::CLIENT_SIDE,b);
    }
    else if(channel_type=="s"){
        cout<<"using a shared memory"<<endl; 
        chan2 = new SHMRequestChannel(channel_name,SHMRequestChannel::CLIENT_SIDE,b); 
    }
   

    //copy_file_from_server(binary_file2,file_path,*chan,"y.csv"); 
    
    

    //RequestChannel* chan2 = new MQRequestChannel("control2", RequestChannel::CLIENT_SIDE); 

    /*
    ////////connecting to server//////////////////
    FIFORequestChannel chan ("control", FIFORequestChannel::CLIENT_SIDE);

    ///////////////////copy all of data points/////////////
    data_points_files_(patients,file_path,chan);
    ///NOTE: to copy individual data points use the following:
    /*
    int patient_ID;
    double time_ ; 
    int ecg; 
    
    while(true){
        cout<<"*****Enter patient, time, ecg *******\n";
        cin>>patient_ID>>time_>>ecg;
        if(patient>15 || patient<1){
            break; 
        } 
        
        cout<<"requested value "<<patient_data_points(patient_ID,time_,ecg,file_path,chan)<<"\n";
    }
    */

    /////////////////copy text file ////////////////////
    //NOTE: the last argument takes the type of file
    // by defualt it has no type///
    //copy_file_from_server(fileName,file_path,chan,"y.csv" );

    ///copy bynary file///////////
    /*
    copy_file_from_server(binary_file,file_path,chan,"y.dat"); 
    copy_file_from_server(binary_file2,file_path,chan,"y.dat"); 
    //////////////////////////////

    ///create a new channel and return name /////////////

    string channel_name = get_new_channel(chan); 
    cout<<"\n\n\n\n\nNEW CHANNEL NAME - > "<<channel_name<<"\n\n\n\n\n\n"; 
    FIFORequestChannel chan2( channel_name,FIFORequestChannel::CLIENT_SIDE);

    
    /////////////////copy file using the provided channel/////////////
    copy_file_from_server(binary_file3,file_path,chan2,"ky.dat"); 
    copy_file_from_server(binary_file4,file_path,chan,"y.dat"); 

    /////////////////send quit signal/////////
    */
    int status; 


    shut_channel(*chan2); ///closes second channel

    shut_channel(*chan); /// closes initial channel

    while( wait(&status)!=pid)
    ;


    cout<<"completely shut down channels "<<endl; 
    delete chan; 
    delete chan2; 

    /*
    cout<<"new test begins here "<<endl ;

    RequestChannel * chan3 = new SHMRequestChannel("control_sh",RequestChannel::CLIENT_SIDE,1024); 

    cout<<"\nrequested value "<<patient_data_points(1,0,2,file_path,*chan3)<<"\n";
    cout<<"about to delete the sh_memory"<<endl; 
    delete chan3; */

    }

     
	return 0;
    
}





























//////////////*********CLIENT FUNCTION DECLARATION*************/////////////////

int copy_file_from_server(string fileName,string file_path,RequestChannel & chan,string file_type1){
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
    string finished_file_name = file_path+fileName.substr(0,1)+file_type1; 

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
double patient_data_points(int patient,double time, int ecg,string file_path,RequestChannel& chan ){ 
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



string get_new_channel(RequestChannel & chan){
    MESSAGE_TYPE message = NEWCHANNEL_MSG; 

    char* add_new_channel = (char*) &message; 

    chan.cwrite(add_new_channel,sizeof(MESSAGE_TYPE) ); 
    int length =0; 
    char * buffer_channel = chan.cread(&length); 

    string channel_name = buffer_channel; 

    return channel_name; 
}


int shut_channel(RequestChannel& chan2){

    MESSAGE_TYPE message = QUIT_MSG; 

    char* add_new_channel = (char*) &message; 
    chan2.cwrite(add_new_channel,sizeof(MESSAGE_TYPE) );
    
    return 0; 
}