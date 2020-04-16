
#include "KernelSemaphore.h"

KernelSemaphore::KernelSemaphore(string semaphore_name, int val){
    this->semaphore_name = semaphore_name; 
    this->semaphore_ptr = sem_open(semaphore_name.c_str(),O_CREAT,0644,0); 

}
void KernelSemaphore::P(){
    //cout<<"testing"<<endl; 
    sem_wait(semaphore_ptr); 
}

void KernelSemaphore::V(){
    //cout<<"testing2 "<<endl;
    sem_post(semaphore_ptr);  
}

KernelSemaphore::~KernelSemaphore(){
    //cout<<"attemp to close "<<semaphore_name<<endl; 
    sem_close(semaphore_ptr); 
    sem_unlink(semaphore_name.c_str()); 
    //cout<<"semaphore destructor"; 
}