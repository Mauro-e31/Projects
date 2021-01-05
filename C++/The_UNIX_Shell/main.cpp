
#include<unistd.h>
#include"parser.h"
#include"my_shell.h"


int main(int argc, char const *argv[])
{
    cout<<"$:"; 
    vector<vector<string>> cmd_with_args= parse_cmd_line();
 
    
    while(cmd_with_args[0][0]!= "exit"){
        pid_t pid = fork();
        if(pid ==0){
            ///shell programs to be executed///
            //char* args[] ={"ls","-a",NULL};
            //string flags = get_args_to_string(cmd_with_args[0]); 
            //cout<<"flags = "<<flags<<endl; 
            //execlp(cmd_with_args[0][0].c_str(),flags.c_str());
            execlp("ls","ls","-l",(char*) NULL); 

            return 0; 
        }
        else if(pid>0){
            //
            wait(NULL);
            cout<<"$:"; 
            cmd_with_args=parse_cmd_line(); 
        }
        else{
            cout<<"fork failed"<<endl ;
            return 1; 
        }
    }

    return 0;
}