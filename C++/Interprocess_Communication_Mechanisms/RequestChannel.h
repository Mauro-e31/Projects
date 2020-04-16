#ifndef _RequestChannel_H_
#define _RequestChannel_H_


class RequestChannel{
    public:
    typedef enum {SERVER_SIDE,CLIENT_SIDE} Side  ; 
    typedef enum { READ_MODE, WRITE_MODE} Mode; 
    string my_name;
	Side my_side;
    int max_buffer_size=256;

    RequestChannel(const string _name, const Side _side, int b_size): 
    my_name(_name),my_side(_side),max_buffer_size(b_size)
    {
        
    };
    //RequestChannel(){}; 
    virtual ~RequestChannel(){

    };   
  
    virtual char * cread(int *len=NULL) =0; 
 
    virtual int cwrite(char *msg, int msglen) =0; 
};

#endif