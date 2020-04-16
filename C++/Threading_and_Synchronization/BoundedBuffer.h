#ifndef BoundedBuffer_h
#define BoundedBuffer_h

#include <stdio.h>
#include <queue>
#include <string>
#include <pthread.h>

using namespace std;

class BoundedBuffer
{
private:
  int cap;
  int total_request; 
  int completed_request=0; 
  queue< vector<char> > main_buffer; 
  pthread_mutex_t mut; 
  pthread_cond_t prod_done=PTHREAD_COND_INITIALIZER, cons_done=PTHREAD_COND_INITIALIZER; 

public:
	BoundedBuffer(int _cap){
		this->cap = _cap; 
		pthread_mutex_init(&(this->mut),NULL);
		//pthread_cond_t cond = 
	}
	~BoundedBuffer(){

	}

	void set_request_amount(int x){
		total_request= x; 
		return; 
	}


	int get_completed_request(){
		return completed_request; 
	}

	int get_total_request(){
		return total_request; 
	}

	void push(char* data, int len){

		pthread_mutex_lock (&(this->mut)); 
		//cout<<"push functioin"<<endl; 
		//converting to vector<char>//
		while ( cap ==main_buffer.size() ){
			//cout<<"still here "<<endl; 
			pthread_cond_wait(&cons_done, &mut); 
		}

		vector<char> new_request; 
		for(int i =0; i<len;i++){
			char part_of_request= *data; 
			data++; 
			new_request.push_back(part_of_request); 
		}

		main_buffer.push(new_request); 
		//total_request++; 
		pthread_cond_signal(&prod_done); 

		pthread_mutex_unlock(&(this->mut)); 
		
	}

	vector<char> pop(){
		pthread_mutex_lock(&mut ); 
		while(main_buffer.size()==0){
			//cout<<"size is zeroooooooo"<<endl;
			/*cout<<"completed_reques = "<<completed_request<< " total_request = "<<total_request<<endl; 
			if (completed_request == total_request)
			{
				pthread_cond_signal(&cons_done);
				pthread_mutex_unlock(&mut);
				vector<char> temp;
				return temp;
			}*/
			pthread_cond_wait(&prod_done,&mut);
			
			
			//cout<<"yo "<<endl; 
		}
		vector<char> t= main_buffer.front(); 
		main_buffer.pop(); 
		completed_request=completed_request+1; 
		pthread_cond_signal(&cons_done); 
		pthread_mutex_unlock(&mut); 
		return t;  
	}

	int get_size(){
		//pthread_mutex_lock(&mut );
		int size = main_buffer.size(); 
		//pthread_mutex_unlock(&mut);
		return main_buffer.size();//size; 

		
	}

	bool is_empty(){
		bool empty =false; 
		pthread_mutex_lock(&mut);
		if(main_buffer.size()==0){
			empty=true;
		}
		pthread_mutex_unlock(&mut); 
		return empty; 

	}


};

#endif /* BoundedBuffer_ */
