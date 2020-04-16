#ifndef _KernelSemaphore_H_
#define _KernelSemaphore_H_

#include "common.h"

class KernelSemaphore{
private: 
string semaphore_name;
//sem_t* semaphore_ptr;  
int semap_val=0; 

public: 
KernelSemaphore(string semaphore_name, int val); 
~KernelSemaphore(); 
sem_t* semaphore_ptr;  


void P(); 
void V(); 
};


#endif