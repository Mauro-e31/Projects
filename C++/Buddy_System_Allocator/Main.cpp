#include "Ackerman.h"
#include "BuddyAllocator.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

int main(int argc, char ** argv) {

    /* We start the program by parsing all arguments passed */
    string  input_value;
    int basic_block_size = 128, memory_length = 128*1024*1024;
    for( int i =1; i<argc; i ++){
        input_value = argv[i];
        if(input_value== "-b"){
            //cout<<"block size = "<<argv[i+1]<<endl;
            basic_block_size= std::stoi(argv[i+1]);
        }
        else if(input_value== "-m"){
            //cout<<"memory size = "<<argv[i+1]<<endl;
            memory_length = std::stoi(argv[i+1]);
        }
        else{
            basic_block_size = 128, memory_length = 128*1024*1024;
        }
    }

    /*once we have our basic block size and memory lenght we can pass it into our
     *own memory allocator and begin testing */ 


  // test memory manager
  BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);
  
  /*running test on ackerman code */
  Ackerman* am = new Ackerman ();
  am->test(allocator); // this is the full-fledged test.

  // destroy memory manager
  delete allocator;
}
