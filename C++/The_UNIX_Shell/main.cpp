
#include<unistd.h>
#include"parser.h"
#include"my_shell.h"


int main(int argc, char const *argv[])
{
    cout<<"$:"; 
    vector<vector<string>> cmd_with_args= parse_cmd_line(); 
    
    while(true){
        if(cmd_with_args[0].size()>0 && cmd_with_args[0][0]== "exit"){
            return 0; 
            ///end program 
        }


        pid_t pid = fork();

        if(pid ==0){
            ///shell programs to be executed///

            ///if we have a special operator such as a & 
            // the program runs in the background_so we fork again
            if(cmd_with_args[0].size()!=0 && cmd_with_args[0].back()=="&"){
                cout<<"here bro bro "<<endl; 
                //program to be executed in the background 
                cmd_with_args[0].pop_back(); //remove & from list
                execute_in_background(cmd_with_args[0]);
                return 0; 
                
            }

            else if(cmd_with_args.size()<=1){ /// otherwise a program is run iin the foreground
                run_program(cmd_with_args[0]); 

            }

            else{

                ///child of main fork() 
                print_cmd_with_args(cmd_with_args); 
                int fd[2]; 

                pipe(fd); 

                


            }
            

            

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


    }/////while loop 

    return 0;
}