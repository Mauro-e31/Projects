/*
    File: my_allocator.cpp
*/
#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
	this->memory= new char[_total_memory_length];
	this->_total_memory_length= _total_memory_length;
  	this->_basic_block_size= _basic_block_size;


  BlockHeader* new_header = (BlockHeader * ) memory;
  ////cout<<"Available memory = "<< _total_memory_length - sizeof(BlockHeader)<<"\n";
  new_header->is_used= false;
  new_header->block_size = _total_memory_length;
  new_header->next_header= NULL;

  char * test_user_buffer= memory+16;
  char * test2 = ((char*)new_header)+16;
  ////cout<<"user_buffer1 -> "<<static_cast<void*>(test_user_buffer)<<endl;
  ////cout<<"user_buffer2 -> "<<static_cast<void*>(test2)<<endl;
  /*create a free list value*/
  for(int i =_basic_block_size; i<=_total_memory_length; i=i*2 ){
	  LinkedList* temp_ptr= new LinkedList();
	  temp_ptr->set_block_size(i);
	  free_list.push_back(temp_ptr);
  }

  free_list.back()->insert(new_header);
  //print_free_list();

}

BuddyAllocator::~BuddyAllocator (){
	for(int i =0; i<free_list.size(); i++ ){
		delete free_list[i];
    }
delete [] memory;
}

char* BuddyAllocator::alloc(int _length) {
  /* This preliminary implementation simply hands the call over the
     the C standard library!
     Of course this needs to be replaced by your implementation.
  */
  //cout<<"before allocating \n";
  //print_free_list();
 int memory_needed = NearestPow2(_length + sizeof(BlockHeader));
 //cout<<"\nmemory_needed by user = "<<memory_needed<<endl;
 if(memory_needed<_basic_block_size){
	 memory_needed= _basic_block_size;
 }

 if(is_block_available(memory_needed)){
	 return ( get_BH_and_pop_from_FL(memory_needed)+16);
 }


 for(int j =memory_needed*2; j<=_total_memory_length;j=j*2 ){
	 //cout<<"trying to find memory with value = "<<j<<"\n";

	 if(is_block_available(j)){
		 char *temp_mem_address= get_BH_and_pop_from_FL(j);
		 BlockHeader* temp_ptr2= (BlockHeader*)temp_mem_address;
		 BlockHeader* splited_block = split( temp_ptr2);
		 get_LL_address_of_bsize(j/2)->insert_buddies(splited_block);

		 if(is_block_available(memory_needed)){
			 return ( get_BH_and_pop_from_FL(memory_needed)+16);
		 }
		 else{
			 j= memory_needed;
		 }
	 }

 }
 ////cout<<"cannot find memory.. perhaps no more memory left :/  \n";
 return NULL;

}

int BuddyAllocator::free(char* _a) {
	//cout<<"////////////////////////CALLING-FREE///////////////////////////////\n\n";
	BlockHeader * block_to_be_free = (BlockHeader*) (_a-16);
	block_to_be_free->is_used = false;
	int free_block_size = block_to_be_free->block_size;
	int location = (_a-memory) ;
	//cout<< "location of a "<< location <<endl;
	//cout<< "cleaning up.... "<<endl;
	//print_free_list();

	//cout<<"adding block of size = "<<free_block_size<< " to the free list\n";
	//cout<<"is his buddy free? .. \n";
	//print_free_list();
	BlockHeader * possible_buddy = getbuddy(block_to_be_free);
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

	  }else{
		  for(int i =location; i<free_block_size; i++){
			  memory[i]= '\0';
		  }
		get_LL_address_of_bsize(free_block_size)->insert(block_to_be_free);
		//cout<<"buddy is occupied \n\n"<<endl;
	}
	//print_free_list();

  /* Same here! */
  //delete _a;
  //cout<<"exiting free\n\n";
  _a = NULL;
  return 0;
}

void BuddyAllocator::debug (){

}

void BuddyAllocator::print_free_list(){
	//cout<<"printing the free list: \n\n";
	for(int i =0; i< free_list.size(); i++){
		cout<<free_list[i]->get_block_size()<<" -> "; //<<" -> "<<free_list[i]->get_head()<<endl;
		free_list[i]->print_linked_list();
		cout<<"\n";
	}
}

bool BuddyAllocator::is_block_available(int _length){
	for(int i =0; i<free_list.size(); i++){
		if( (free_list[i]-> get_head() != NULL) && (free_list[i]->get_block_size()== _length)){
			//cout<< "BLOCK of size =  "<<_length<< ", Available\n" ;
			return true;
		}
	}
	return false;
}

LinkedList* BuddyAllocator::get_LL_address_of_bsize( int _length){
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
}

BlockHeader* BuddyAllocator::getbuddy(BlockHeader * addr){
	//cout<<"\n////////////////////////////!!!!!!**********!!!!!!!/////////////\n";
	//cout<<"/////////////////---CHECKING FOR BUDDY---/////////////////////////\n";
	int buddy1_size = addr->block_size;
	char* updated_addr= (char*) addr;
	//cout<<"offset value = "<<static_cast<void*>(memory)<<"\n";
	//cout<< "current value = "<<updated_addr-memory<<"\n";

	//print_free_list();
	LinkedList * list_ = get_LL_address_of_bsize(buddy1_size);
	BlockHeader* list_head= list_->get_head();
	BlockHeader* index_ = list_head;
	while(index_){
		if( arebuddies( addr, index_) ){
			return index_;
		}
		index_ = index_->next_header;
	}

	return NULL;

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
