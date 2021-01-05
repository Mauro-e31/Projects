#include"parser.h"

vector<string>get_tokens(string cmd_ln){
    istringstream cmd_stream(cmd_ln); 
    string token; 
    vector<string> token_vector; 
    while(cmd_stream>>token){
        token_vector.push_back(token); 
    }
    return token_vector; 
}

void print_cmd_tokens(vector<string> tokens){
    for(int i =0; i<tokens.size(); i++){
        cout<<tokens[i]<<"**"; 
    }
    cout<<endl;
    return; 
}

vector<vector<string>> get_cmds_with_args(vector<string> tokens){
    vector<vector<string>> cmds_and_args; 

    //assume the cmd_line has split the user command into tokens
    vector<string> temp_list; 
    for(int i =0; i<tokens.size(); i++){
        string current_token= tokens[i]; 

        temp_list.push_back(current_token); 
    
        if(special_operator(current_token)){
            cmds_and_args.push_back(temp_list); 
            vector<string> new_list; 
            temp_list=new_list; 
        }
    }

    
    cmds_and_args.push_back(temp_list); 

    return cmds_and_args; 
}

bool special_operator(string token){
    vector<string> special_ops={"|","&"}; 
    for(int i =0;i<special_ops.size(); i++){
        if(token==special_ops[i]){
            return true; 
        }
    }
    return false; 
}


void print_cmd_with_args(vector<vector<string>> cmds){
    cout<<"Now printing parsed commands with arguments..."<<endl ;
    for(int i =0; i <cmds.size(); i++){

        for(int j=0; j<cmds[i].size(); j++){
            cout<<cmds[i][j]<<" "; 

        }
        cout<<endl; 
    }
    return; 
}

vector<vector<string>>parse_cmd_line(){
    string usr_cmd_line; 
    getline(cin,usr_cmd_line);

    vector<string> tokens=get_tokens(usr_cmd_line); 

    vector<vector<string>> cmds_with_args= get_cmds_with_args(tokens); 

    return cmds_with_args; 

    //print_cmd_with_args(cmds_with_args); 
}