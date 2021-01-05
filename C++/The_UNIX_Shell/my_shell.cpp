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