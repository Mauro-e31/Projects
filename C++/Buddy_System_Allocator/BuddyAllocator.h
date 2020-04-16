/*
    File: my_allocator.h

    Original Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

 */

#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
using namespace std;
typedef unsigned int uint;

/* declare types as you need */

class BlockHeader{
	// decide what goes here
	// hint: obviously block size will go here
	public:
	uint block_size;
	bool is_used;
	BlockHeader * next_header;
};

class LinkedList{
	// this is a special linked list that is made out of BlockHeader s.
private:
	BlockHeader* head= NULL;
	int block_size;
	BlockHeader* tail= NULL;  		// you need a head of the list
public:
	void insert (BlockHeader* b){	// adds a block to the list
		if(head==NULL){
			head=b;
			tail=b;
			head->next_header=NULL;
		}
		else{

			BlockHeader * temp_ptr= tail;
			tail = b;
			tail->next_header=NULL;
			temp_ptr->next_header= tail;
		}

	}
	void insert_buddies(BlockHeader * b ){
		if(head==NULL){
			head=b;
			tail=b->next_header;
			//head->next_header=NULL;
		}
		else{

			BlockHeader * temp_ptr= tail;
			tail = b->next_header;
			tail->next_header=NULL;
			temp_ptr->next_header= b;
		}
	}

	void remove (BlockHeader* b){  // removes a block from the list
		if(b==head && (b==tail)){
			//BlockHeader*temp_ptr= b->next_header;
			//delete head;
			//head = temp_ptr;
			head = NULL;
			tail = NULL;
		}
		else if (head==b) {
			BlockHeader* temp_ptr = head;
			head = temp_ptr->next_header;
		}
		else{
			BlockHeader* temp_ptr= head;
			while(temp_ptr){
				if(temp_ptr->next_header== b){
					if(temp_ptr->next_header->next_header!=NULL){
						temp_ptr->next_header= temp_ptr->next_header->next_header;
					}
					else{
						temp_ptr->next_header= NULL;
						tail= temp_ptr;
					}
				}
				else{
					temp_ptr= temp_ptr->next_header;
				}
			}
		}
	}

	/*
	this functions used for debugging. not actually being called;
	BlockHeader* check_for_buddy( BlockHeader * buddy1){/// linked list:
		cout<<"\n////////////////////////////!!!!!!**********!!!!!!!/////////////\n";
		cout<<"/////////////////---CHECKING FOR BUDDY---/////////////////////////\n";

		BlockHeader * index_= head;
		unsigned long b_size = buddy1->block_size;
		while(index_){
			char * b1 = (char*) buddy1;
			char * b2 = (char*) index_;
			unsigned long possible_buddy = ( (unsigned long)b1 ^ (unsigned long)b2 );
			if (  possible_buddy ==b_size){
				cout<<"found his buddy "<<endl;
				return index_;
			}
			index_= index_->next_header;
		}
		return NULL;
	}*/
	bool available_block(int b_size){
		BlockHeader* temp_ptr= head;
		while(temp_ptr != NULL){
			if(temp_ptr->block_size== b_size ){
				return true;
			}
			else{
				//cout<<"checking here "<<endl;
				temp_ptr= temp_ptr->next_header;
				//cout<<"checking over here "<<endl;
			}
		}
		return 0;

	}

	void set_block_size(int b){
		this->block_size= b;
	}
	int get_block_size(){
		//cout<<"got the block size "<< block_size<<endl;
		return this->block_size;
	}
	BlockHeader * get_head(){
		return this->head;
	}

	void print_linked_list(){
		BlockHeader* temp_ptr=head;
		if(temp_ptr==NULL){
			cout<<0;
			return;
		}
		while(temp_ptr){
			cout<<temp_ptr;
			temp_ptr=temp_ptr->next_header;
			if(temp_ptr!=NULL){
				cout<< " -> ";
			}
		}
	}



};


class BuddyAllocator{
private:
	/* declare member variables as necessary */
	char * memory;
	int _basic_block_size;
	vector<LinkedList* > free_list;
	int _total_memory_length;


private:
	/* private function you are required to implement
	 this will allow you and us to do unit test */

	BlockHeader* getbuddy (BlockHeader * addr);
	// given a block address, this function returns the address of its buddy

	bool arebuddies (BlockHeader* block1, BlockHeader* block2);
	// checks whether the two blocks are buddies are not

	BlockHeader* merge (BlockHeader* block1, BlockHeader* block2);
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around

	BlockHeader* split (BlockHeader* block);
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected


public:
	BuddyAllocator (int _basic_block_size, int _total_memory_length);
	/* This initializes the memory allocator and makes a portion of
	   ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as
	   its minimal unit of allocation. The function returns the amount of
	   memory made available to the allocator. If an error occurred,
	   it returns 0.
	*/
	~BuddyAllocator();
	/* Destructor that returns any allocated memory back to the operating system.
	   There should not be any memory leakage (i.e., memory staying allocated).
	*/

	char* alloc(int _length);
	/* Allocate _length number of bytes of free memory and returns the
		address of the allocated portion. Returns 0 when out of memory. */

	int free(char* _a);
	/* Frees the section of physical memory previously allocated
	   using ’my_malloc’. Returns 0 if everything ok. */

	void debug ();
	/* Mainly used for debugging purposes and running short test cases */
	/* This function should print how many free blocks of each size belong to the allocator
	at that point. The output format should be the following (assuming basic block size = 128 bytes):

	128: 5
	256: 0
	512: 3
	1024: 0
	....
	....
	 which means that at point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/

	 void print_free_list();
	 bool is_block_available(int x);
	 LinkedList* get_LL_address_of_bsize(int _length);
	 char* get_BH_and_pop_from_FL(int );
};


int NearestPow2(int number);
#endif
