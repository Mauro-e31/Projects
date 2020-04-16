
## Project Description: 

In this programming assignment, I was to develop my own memory allocator that was based on the buddy system memory allocation scheme. In short I was given the task to create my own versions of ***malloc()*** and ***free()*** from the standard C library. These were to be called **BuddyAllocator::alloc()** and **BuddyAllocator::free()**. 

### Objectives: 
* Package a simple module with some static data as separately compiled unit
* Become Deeply familiar with pointer management and array management in the C/C++ languages
* Become familiar with standard methods for handling command-line arguments in a C/UNIX environment. 
* Become fmiliar with simple UNIX development tools such as: the compiler, make, the GNU debugger, object file inspector,etc..

### How to run program:
Ackerman.h and Ackerman.cpp were provided to me to test my program.
program takes arguments in the form of: <br> **memtest [-b \<blocksize>] [-s \<memsize>]**

To run the code do the following commands: 
* clear any .o files by typing **```make clean```**
* Compile the code by typing **``make``** this will create the program called memtest.
* To run the program type **``./memtest``** by default memtest will run a test with a block size of 128 bytes and memory size of 512 kB
* example of changing block size and memory size **``./memtest -b 256 -s 1024``**
