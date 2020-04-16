/*
    File: my_allocator.cpp
	Author: Mauro Lopez 
	//Code is based on buddyAllocator.h
*/
#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
	this->memory = new char[_total_memory_length];
	//NOTE: we use a char since its memory size is 1 byte

	this->_total_memory_length = _total_memory_length;
	this->_basic_block_size = _basic_block_size;

	/*we have allocated a chunk of memory from the heap*
	 *This will be the memory we will use for the remaining of the program*/

	///---Chaning our memory into our block header to easily manipulated--//////
	BlockHeader *new_header = (BlockHeader *)memory;

	//Now we can begin by making our first memory block available
	new_header->is_used = false;
	new_header->block_size = _total_memory_length;
	new_header->next_header = NULL;


	/*Note: the BuddyAllocator class has a private member variable called "free_list"*
	 *This is a vector of LinkedList pointers, this pointers will point to a list of 
	 *blocks that are available for the user to use. 
	 *Since the memory and blocks can be different depending on the users input, a *
	 *for loop is used to adapt for this changes */
	 
	for (int i = _basic_block_size; i <= _total_memory_length; i = i * 2)
	{	
		/*create a list with the basic_block size then multiply by two*/
		LinkedList *temp_ptr = new LinkedList();
		temp_ptr->set_block_size(i);
		free_list.push_back(temp_ptr);
	}

	free_list.back()->insert(new_header);
	//We add our first block to the free list

	//print_free_list(); //used to debug and print list of available blocks
}

BuddyAllocator::~BuddyAllocator (){
	for(int i =0; i<free_list.size(); i++ ){
		delete free_list[i];
    }
delete [] memory;
}

char* BuddyAllocator::alloc(int _length) {

 int memory_needed = NearestPow2(_length + sizeof(BlockHeader));
 if(memory_needed<_basic_block_size){
	 memory_needed= _basic_block_size;
 }

 /* memory needed is equal to the users need plus the extra we use in our block header 
  * We round this to the nearest power of 2 or to the basic block size */

 if(is_block_available(memory_needed)){
	 return ( get_BH_and_pop_from_FL(memory_needed)+16);
 }

 /*if the desired block is found in the free list, then we get the address of that *
  *block, remove it from the free list and return the address. The "+16" is use to move *
  *our pointer to the available memory for the user, this prevents the user from *
  *overriding the blocks information such as block size etc. */


 /*if no memory block is availble with the a certain length, then we need to start looking*
  *for bigger blocks to split */
 
 for(int j =memory_needed*2; j<=_total_memory_length;j=j*2 ){
	 //cout<<"trying to find memory with value = "<<j<<"\n";

	 if(is_block_available(j)){ 
		 char *temp_mem_address= get_BH_and_pop_from_FL(j); //get the address of the Block header and remove from Free list
		 BlockHeader* temp_ptr2= (BlockHeader*)temp_mem_address; // use a block header pointer to point to the block 
		 BlockHeader* splited_block = split( temp_ptr2); // pass the block address and return the address of the Left most buddy 
		 get_LL_address_of_bsize(j/2)->insert_buddies(splited_block); // go to our Free list and look for a block with the new
		 // block size. and insert both buddies in the free list. 


		 if(is_block_available(memory_needed)){
			 return ( get_BH_and_pop_from_FL(memory_needed)+16); //if by spliting a bigger block we find our 
			 //desired block length return that block address plus the offset of the header info
		 }
		 else{
			 j= memory_needed; //if the block we split up still too big for the desired block, 
			 //we search again to see if the block we just split could get us closer to our goal 
		 }
	 }

 }
 ////cout<<"cannot find memory.. perhaps no more memory left :/  \n";
 return NULL;

}

int BuddyAllocator::free(char* _a) {
	//////////////////////////CALLING-FREE///////////////////////////////
	BlockHeader * block_to_be_free = (BlockHeader*) (_a-16); //delete offset to return to beginning of block, and header info
	block_to_be_free->is_used = false; 
	int free_block_size = block_to_be_free->block_size;
	int location = (_a-memory) ;

	//print_free_list();

	//once free we need to see if his buddy is also empty

	//cout<<"is his buddy free? .. \n";
	//print_free_list();
	BlockHeader * possible_buddy = getbuddy(block_to_be_free); //check if buddy is empty
	if(possible_buddy){
		BlockHeader* new_big_block = merge(block_to_be_free, possible_buddy);
		while(getbuddy(new_big_block)){
			//cout<<"block that just merged has a buddy, now merging with its buddy \n";
			BlockHeader* bigger_buddy = getbuddy(new_big_block);
			new_big_block= merge(new_big_block,bigger_buddy);
			////print_free_list();
		}
		int big_block_location =  ((char*)new_big_block+16)-memory;
		for(int i =big_block_location; i<new_big_block->block_size; i++){
			memory[i]= '\0';
		}

		get_LL_address_of_bsize(new_big_block->block_size)->insert(new_big_block);

	  }else{ //no buddy is found so the memory is wiped but not merged with the other buddy. 
		  for(int i =location; i<free_block_size; i++){
			  memory[i]= '\0'; // setting the memory back to zero
		  }
		get_LL_address_of_bsize(free_block_size)->insert(block_to_be_free); //put the empty block back to the free list. 
		//cout<<"buddy is occupied \n\n"<<endl;
	}
	//print_free_list();

  _a = NULL;
  return 0;
}

void BuddyAllocator::debug (){

}

void BuddyAllocator::print_free_list(){

	//function used for debugging, have a visual of whats going on the free list. 
	//cout<<"printing the free list: \n\n";
	for(int i =0; i< free_list.size(); i++){
		cout<<free_list[i]->get_block_size()<<" -> "; //<<" -> "<<free_list[i]->get_head()<<endl;
		free_list[i]->print_linked_list();
		cout<<"\n";
	}
}

bool BuddyAllocator::is_block_available(int _length){
	//check the freelist for a block of size _lenth

	for(int i =0; i<free_list.size(); i++){
		if( (free_list[i]-> get_head() != NULL) && (free_list[i]->get_block_size()== _length)){
			//cout<< "BLOCK of size =  "<<_length<< ", Available\n" ;
			return true;
		}
	}
	return false;
}

LinkedList* BuddyAllocator::get_LL_address_of_bsize( int _length){

	//Returns for the link list address of the blocks with size _length
	for(int i =0; i<free_list.size(); i++){
		if( (free_list[i]->get_block_size()== _length) ){
			return (free_list[i]);
		}
	}
	////cout<<"not supposed to happen \n";
	return NULL;
}

char * BuddyAllocator::get_BH_and_pop_from_FL(int _length){
	for(int i =0; i<free_list.size(); i++){
		if( (free_list[i]-> get_head() != NULL) && (free_list[i]->get_block_size()== _length)){
			LinkedList* temp_ptr= free_list[i];
			free_list[i]->get_head()->is_used = true;
			BlockHeader* temp_header = free_list[i]->get_head();
			free_list[i]->remove(temp_header);
			//cout<<"getting block header and poping from Free list\n\n";
			//print_free_list();
			return ((char*)temp_header);
		}
	}
	return NULL; 
}

BlockHeader* BuddyAllocator::getbuddy(BlockHeader * addr){
	//cout<<"\n////////////////////////////!!!!!!**********!!!!!!!/////////////\n";
	//cout<<"/////////////////---CHECKING FOR BUDDY---/////////////////////////\n";
	int buddy1_size = addr->block_size;
	char* updated_addr= (char*) addr; //address of block

	//print_free_list();

	/*we are going to go through the free list of a certain size and search for the buddy*/

	LinkedList * list_ = get_LL_address_of_bsize(buddy1_size); //get our linklist address. 
	BlockHeader* list_head= list_->get_head();// return first element in the link list
	BlockHeader* index_ = list_head; //start our search here
	while(index_){
		if( arebuddies( addr, index_) ){ //functions check if the current address matches the block 
			return index_; //if it matches, then the buddy is found and returned
		}
		index_ = index_->next_header;
	}

	return NULL; // no buddy waas found 

}

BlockHeader * BuddyAllocator::split(BlockHeader* block){
	//cout<<"////////////////////---calling split---///////////////\n\n";
	int _split_size = block->block_size/2;
	block->block_size= _split_size;
	block->is_used = false;
	char* temp_block_memory = (char*) block;
	BlockHeader * block_buddy = (BlockHeader*)(temp_block_memory + (_split_size) );
	block->next_header= block_buddy;
	block_buddy ->block_size= _split_size;
	block_buddy ->is_used = false;
	block_buddy ->next_header=NULL;

	return block;

}

int NearestPow2( int number){
  int temp = number%2;
  //////cout<<"temp = "<<temp;
  int exp_value = ( log2(number- temp)) +1;
  int nearest_value = pow(2,exp_value);
  return nearest_value;

}

bool BuddyAllocator::arebuddies( BlockHeader* block1, BlockHeader* block2){
	////cout<<"\n////////////////////////////!!!!!!**********!!!!!!!/////////////\n";
	////cout<<"/////////////////---CHECKING FOR BUDDY 2---/////////////////////////\n";

	unsigned long b_size = block1->block_size;
		char * b1 = (char*) block1;
		char * b2 = (char*) block2;
		unsigned long possible_buddy = ( (unsigned long)b1 ^ (unsigned long)b2 );
		if (  possible_buddy ==b_size){
			////cout<<"found his buddy "<<endl;
			return true;
		}


	return false;
}

BlockHeader* BuddyAllocator::merge(BlockHeader* block1, BlockHeader* block2 ){
	////cout<<"\n*****************************************************************\n";
	////cout<<"/////////////////---CALLING MERGE FUNCTION---/////////////////////////\n";
	////cout<<"before merege \n\n";
	int free_block_size= block1->block_size;
	////print_free_list();
	//get smallest address first //
	char* buddy1 = (char*)block1;
	char* buddy2 = (char*)block2;
	int location = ((buddy1+16) - memory);
	if(buddy1<buddy2){
		////cout<<"smallest address = buddy1 -> "<<static_cast<void*>(buddy1)<<endl;
		get_LL_address_of_bsize(free_block_size)->remove(block2);
		block1->block_size= free_block_size*2;
		block1->is_used= false;
		/*for(int i =location; i<free_block_size*2; i++){
			memory[i]= '\0';
		}*/
		//get_LL_address_of_bsize(free_block_size*2)->insert(block1);
		return block1;
	}
	else{
		////cout<<"smallest address = buddy2 -> "<<static_cast<void*>(buddy2)<<endl;
		get_LL_address_of_bsize(free_block_size)->remove(block2);
		block2->block_size= free_block_size*2;
		block2->is_used= false;
		int buddy_location = ((buddy2+16) - memory);
		/*for(int i =buddy_location; i<free_block_size*2; i++){
			memory[i]= '\0';
		}*/
		//get_LL_address_of_bsize(free_block_size*2)->insert(block2);
		return block2;
	}

}
