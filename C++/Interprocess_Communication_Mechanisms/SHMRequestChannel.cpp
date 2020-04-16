#include "common.h"
#include "SHMRequestChannel.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

SHMRequestChannel::SHMRequestChannel(const string _name, const Side _side,int b_size):
RequestChannel(_name,_side,b_size)
{
	

    sm1 = "/sm1_" + my_name + "1"; 
    sm2 = "/sm2_" + my_name + "2"; 

	semap1_name = "/semap_" +my_name+ "1"; 
	semap2_name = "/semap_" +my_name+ "2"; 

	

		
	if (_side == SERVER_SIDE){
		wfd = open_shared_mem(sm1, O_RDWR);
		rfd = open_shared_mem(sm2, O_RDWR);

		

		semap_write = new KernelSemaphore(semap1_name,1);
		semap_read = new KernelSemaphore(semap2_name,1); 




	}
	else{
		rfd = open_shared_mem(sm1, O_RDWR);
		wfd = open_shared_mem(sm2, O_RDWR);

		semap_read = new KernelSemaphore(semap1_name,1); 
		semap_write = new KernelSemaphore(semap2_name,1); 
		
	}
	
}

SHMRequestChannel::~SHMRequestChannel()
{ 

	
	close(wfd);
	close(rfd);

	munmap(mem_ptr_read,max_buffer_size); 
	munmap(mem_ptr_write,max_buffer_size); 
	shm_unlink(sm1.c_str() ); 
	shm_unlink(sm2.c_str() ); 


	delete semap_read; 
	delete semap_write; 
	
}


char* SHMRequestChannel::cread(int *len)
{
	//reading 
	//cout<<"inside the cread from the -> "<<my_side<<endl; 
	char * buf = new char [max_buffer_size]; 
	int length=256; 
	int value_ = 9; 
	sem_getvalue(semap_read->semaphore_ptr,&value_); 
	//cout<<value_ << " <- semaphore value "<<endl; 
	semap_read->P(); 
	//cout<<" #after the semaphore "<<endl; 
	mem_ptr_read = (char*) mmap(NULL,max_buffer_size,PROT_READ | PROT_WRITE, MAP_SHARED,rfd,0);
	 
	//strcpy(buf,mem_ptr_read); 
	memcpy(buf,mem_ptr_read,256); 


	if (len)	// the caller wants to know the length
		*len = length;

	//cout<<"exiting cread success, side -> "<<my_side<<endl; 
	return buf;
}

int SHMRequestChannel::cwrite(char* msg, int len)
{	//cout<<"inside the cwrite from the ->"<<my_side<<endl; 

	ftruncate(wfd,max_buffer_size); 
	char * mem_ptr_write = (char * ) mmap(
		NULL,max_buffer_size,PROT_READ | PROT_WRITE, MAP_SHARED, wfd,0);


	if (len > max_buffer_size){
		EXITONERROR("cwrite");
	}
	if ( memcpy(mem_ptr_write,msg,len) < 0){
		EXITONERROR("cwrite");
	}

	

	//cout<<"exiting cwrite success, side -> "<<my_side<<endl; 
	semap_write->V(); 
	return len;
}


int SHMRequestChannel::open_shared_mem(string sh_mem_name, int mode){
    
	int fd = shm_open( sh_mem_name.c_str(), O_CREAT | mode,0644); 

    //open(_pipe_name.c_str(), mode);
	if (fd < 0){
		EXITONERROR(sh_mem_name);
	}
	return fd;
    
}