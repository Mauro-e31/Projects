#include"my_shell.h"

string get_current_dir(){
    char buff[FILENAME_MAX]; 
    getcwd(buff,FILENAME_MAX); 
    string current_working_dir(buff); 
    return current_working_dir; 
}


string get_args_to_string(vector<string> cmd_args){
    string args; 
    if(cmd_args.size()==1){
        return args;  
    }
    for(int i =1; i<cmd_args.size(); i++){
        args.append(cmd_args[i]); 
        args.append(" "); 
    }
    return args; 
}

void execute_in_background(vector<string> cmd_ln){
    //program to be executed in the background 
    cout<<"executing in background"<<endl; 
    //print_cmd_with_args(cmd_with_args); 

    pid_t b_pid= fork(); 

    if(b_pid==0){
        /// program to execute in background 
        run_program(cmd_ln); 
        //child of a processs

        
    }

    else if(b_pid>0){
        ///return to parent
        // parent goes back to its parent 
        // this prevents the main fork from waiting on its 
        // child processes. 
        return ; 
    }
    
    else{
        cout<<"fork failed"<<endl; 
    }
    
    return ; 
}

void run_program(vector<string> cmd_ln){
    char * array_of_strings[cmd_ln.size()+1]; 

    for(int i =0; i<cmd_ln.size(); i++){
        array_of_strings[i]= (char *) cmd_ln[i].c_str(); 
    }
    array_of_strings[cmd_ln.size()]=NULL; 


    //string program_with_arg = get_args_to_string(cmd_with_args[0]); 

    execvp(array_of_strings[0],array_of_strings); 
    return; 
}