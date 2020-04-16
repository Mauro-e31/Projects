#include<iostream>
#include<vector>
#include<string>
#include<bits/stdc++.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>

using namespace std;

int special_command(std::vector<string > );
int valid_string(string);
int unix_command(string);
int command_name(std::vector<string > );
std::vector<string> get_tokens(string);
int redirection(std::vector<string > tokens);
int pipe_command(std::vector<string > );

void execute_comand_name(std::vector<string > );
void execute_redirection(std::vector<string > );
void execute_pipe_command(std::vector<string > );
void execute_special_command(std::vector<string> );

void print_vector_string(std::vector<string > );
void two_pipes(std::vector<std::vector<string > > );
void one_pipe(std::vector<std::vector<string >  > );
void multiple_pipes(std::vector<std::vector<string > > );

void redirections_or_command(std::vector<string> );

void execute_one_unix_command(std::vector<string > temp_cmdline,int & exit);
//void first_pipe(std::vector<string > processes_in_pipes,int & fds[]);



int main(int argc, char const *argv[]) {
  string commd_Line;
  string path="/home/mauro/Documents/csce313/csce_313_pa3";
  cout<<"$:";
  int pid_parent=999;

  //NOTE:shell program main loop
  while( getline(std::cin,commd_Line) ){
    //cout<<"$:";
    /////////////////PARSING PART//////////////////////////////
    ///****************************************************///
    int exit =0;
    std::vector<string> tokens=get_tokens(commd_Line);
    std::vector<std::vector<string > > programs_list;
    std::vector<string> arguments_;

    int num_commands=0;


    for(int i=0; i<tokens.size(); i++){

      arguments_.push_back(tokens[i]);
      if(tokens[i] == "&"){
        num_commands++;
        arguments_.pop_back();
        programs_list.push_back(arguments_);
        arguments_.clear();
      }

    }

    if(!arguments_.empty()){
      programs_list.push_back(arguments_);
    }

    /////////////////PARSING PART//////////////////////////////
    ///****************************************************///

    ////////////////////////start the fork//////////////



    std::vector<int> waiting_blocks;
    std::vector<string> temp_cmdline;
    int index=0;

    if(num_commands>=1){
      cout<<"background process"<<endl;

      int pid_background= fork();


      if(pid_background==0){
        execute_one_unix_command(programs_list[0],exit);
        wait(& pid_parent);
        return 0;
      }
      pid_parent= pid_background;


      //return 0;
    }
    else{
      //cout<<"command_name = "<<programs_list[0][0]<<" "<<programs_list[0][1]<<endl;
      if(programs_list.size()>0){
        if(programs_list[0][0]=="jobs"){
          programs_list[0][0]= "ps";
          programs_list[0].push_back("|");
          programs_list[0].push_back("tail");
          programs_list[0].push_back("-3");
          programs_list[0].push_back("|");
          programs_list[0].push_back("head");
          programs_list[0].push_back("-2");


          execute_one_unix_command(programs_list[0],exit);
        }
        else{
          execute_one_unix_command(programs_list[0],exit);
        }
      }

        //cout<<"pid_parent"<<pid_parent<<endl;
          wait(&pid_parent);
      //  wait(&pid_parent);
    }



/*
    for(int j= 0; j<programs_list.size(); j++){
      temp_cmdline= programs_list[j];

      execute_one_unix_command(temp_cmdline,exit);
      //cout<<"j = "<<j <<endl<<endl;
      /*
      int pid99= fork();
      pid_parent= pid99;

      temp_cmdline= programs_list[j];

      if(pid99==0){
        execute_one_unix_command(temp_cmdline,exit);
        exit++;
        break;
      }






    /*

    if(special_command(temp_cmdline) ){
      chdir(temp_cmdline[1].c_str());

    }

    else{
    int pid = fork();
    pid_parent= pid;

    if(pid==0){

    //for(int i =0; i< programs_list[j].size(); i++){

      if( command_name(temp_cmdline) ){ ///command name with ARGS
        execute_comand_name(temp_cmdline);
      }

      else if( pipe_command(temp_cmdline) ){
        execute_pipe_command(temp_cmdline);
      }/// pipe commands

      else if( redirection(temp_cmdline) ){ // redirection command with ARGS
        execute_redirection(temp_cmdline);
      }
      else if(special_command(temp_cmdline) ){
        exit++;
        break;

      }

      else if(temp_cmdline[0]=="exit"){ /// EXIT special command
        cout<<"Exiting Shell Program....\n";
        exit++;
        break;
      }

      else{//// Invalid Syntax/// wont //close shell
        cout<<"invalid argument "<<endl ;
        exit++;
        break;
      }

    //}//for loop
  }



else{////////////////////parent side of the code/////////////
  //cout<<"waiting for child \n";

  wait(&pid);
  //execute_special_command(temp_cmdline);
  //cout<<"back to shell program "<<endl;
  if(temp_cmdline[0]=="exit"){
    //cout<<"exiting....\n"
    exit++;
    break;
  }

}////end of parent


}
*/
//}/// for loop fork();
/*
if(pid_parent==0){
  return 0;
}*/
//wait(&pid_parent);


//}//other for loop ;


//execute_special_command(temp_cmdline);
//cout<<"back to shell program "<<endl;

if(exit>=1 || index>0 ){
  return 0;
  //break;
}

  //sleep(1);
  cout<<"$:";

  }/// while loop// main shell program



  return 0;
}




































int command_name(std::vector<string> tokens){
  vector<string> commands = {"ls","pwd","echo","mv","cat","ps","grep", "awk","sort","head","dd","sleep","rm"};

  int count =0;
  for(int i =0; i<tokens.size(); i++){
    if( (tokens[i]=="<") || (tokens[i]==">") || (tokens[i]=="|") ){
      return 0;
    }
  }

  for(int i =0; i<commands.size(); i++){
    if(commands[i]==tokens[0]){
      return 1;
  }
}
  return 0;
}

int special_command(std::vector<string > special_){
  if( special_[0]=="cd"){
    return 1 ;
  }

  return 0;

}

int redirection(std::vector<string > tokens){
  if(tokens.size()>=3){

  string destination = tokens[tokens.size()-1];
  string direction = tokens[tokens.size()-2];

  if( (direction=="<") || (direction==">") ){
    return 1;
  }
  else
  return 0;
}

else{
  return 0;
}

}

void execute_special_command(std::vector<string > cmd_line){
  //const char *file_name= cmd_line[0].c_str();

  //cmd_test[cmd_line.size()]=NULL;
  chdir(cmd_line[1].c_str() );


  return ;
}

void execute_comand_name(std::vector<string> cmd_line){
  //cout<<"need to run program called : "<< cmd_line[0]<<"\n";
  //cout<<"arguments = "<<temp_cmdline[1]<<endl;
  //cout<<"argument line = ";print_vector_string(cmd_line);
  const char *file_name= cmd_line[0].c_str();

  char * cmd_test[cmd_line.size()+1];
  for(int i =0; i<cmd_line.size(); i++){
    char const *temp = cmd_line[i].c_str();
    cmd_test[i]=(char*)temp;
  }

  cmd_test[cmd_line.size()]=NULL;
  cout<<execvp(file_name,cmd_test)<<"<-value "<<endl<<endl;
  return;
}



int pipe_command(std::vector<string > tokens){
  for(int i =0; i< tokens.size();i++ ){
    if(tokens[i]=="|"){
      return 1;
    }
  }

  return 0;
}


void execute_redirection(std::vector<string > temp_cmdline){
  std::vector<string > unix_command;
  int left, right =0;
  string direction;

  for(int i =0; i<temp_cmdline.size(); i++){
    direction= temp_cmdline[i];
    if( ( direction== "<") || ( direction== ">") ){
      if(direction== ">"){
        left=0;
        right=1;
      }
      else if(direction=="<"){
        left =1;
        right =0;
      }

      break;
    }

    unix_command.push_back(temp_cmdline[i]);
  }
  //cout<<"command_name size = "<<unix_command.size()<<endl;


  string file_name = temp_cmdline[temp_cmdline.size()-1];

  int fd;
  if(left){
    fd = open(file_name.c_str(), O_RDONLY);
    //string new_cmmd_line;
    dup2(fd,0);
    execute_comand_name(unix_command);
  }

  if(right){
    fd = open(file_name.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    dup2(fd,1);
    execute_comand_name(unix_command);
  }
  //cout<<"executing redirection\n";



  return;
}


void execute_pipe_command(std::vector<string > temp_cmdline){
  cout<<"executing pipe command \n";
  /////count the number of pipes///////
  string token;
  int num_pipes =0;
  std::vector<std::vector<string> > processes_in_pipes;
  std::vector<string > child_process;

  for(int i =0; i<temp_cmdline.size(); i++){
    token= temp_cmdline[i];
    if( token== "|"){
      num_pipes++;
      std::vector<string > temp_child_process = child_process;
      processes_in_pipes.push_back(temp_child_process);

      child_process.clear();

    }
    else{
      child_process.push_back(token);
    }

  }

  std::vector<string > temp_child_process = child_process;
  processes_in_pipes.push_back(temp_child_process);

  switch (num_pipes) {
    case 1:one_pipe(processes_in_pipes);
    case 2: two_pipes(processes_in_pipes);
    default: multiple_pipes(processes_in_pipes);

  }

  return;
}


void print_vector_string(std::vector<string > v){
  for(int i =0; i<v.size(); i++){
    cout<<v.at(i)<<" ";
  }
  cout<<endl;
  return ;
}


void two_pipes(std::vector<std::vector<string > > processes_in_pipes){
  //cout<<"number of pipes requested = "<<num_pipes<<endl;
  cout<<"number of processes in pipe = "<<processes_in_pipes.size()<<endl;
  int temp_fd[2];
  int fd[2];
  pipe(fd);

  int pid_pipe= fork();

  if( pid_pipe== 0){
    close(fd[0]);
    dup2(fd[1],1);
    redirections_or_command(processes_in_pipes[0]);
    execute_comand_name(processes_in_pipes[0]);
  }

  else {
    close(fd[1]);
    wait(&pid_pipe);

    pipe(temp_fd);

    int pid_pipe= fork();

    if(pid_pipe==0){
      close(temp_fd[0]);
      dup2(temp_fd[1],1);
      dup2(fd[0],0);
      redirections_or_command(processes_in_pipes[1]);
      execute_comand_name(processes_in_pipes[1]);
    }

    else{
      wait(&pid_pipe);
      close(temp_fd[1]);
      dup2(temp_fd[0],0);
      redirections_or_command(processes_in_pipes[2]);
      execute_comand_name(processes_in_pipes[2]);

    }

  }
  return;
}


void one_pipe(std::vector<std::vector<string > > processes_in_pipes){
  int fd[2];
  pipe(fd);

  int pid_pipe= fork();

  if( pid_pipe== 0){
    close(fd[0]);
    dup2(fd[1],1);
    redirections_or_command(processes_in_pipes[0]);
    execute_comand_name(processes_in_pipes[0]);
  }

  else {
    close(fd[1]);
    wait(&pid_pipe);
    dup2(fd[0],0);
    redirections_or_command(processes_in_pipes[1]);
    execute_comand_name(processes_in_pipes[1]);
    }

  return;
}

void multiple_pipes(std::vector<std::vector<string > > processes_in_pipes){
  //cout<<"number of pipes requested = "<<num_pipes<<endl;
  //cout<<"number of processes in pipe = "<<processes_in_pipes.size()<<endl;

  int temp_fd[2];
  int fd[2];
  pipe(fd);

  int pid_pipe= fork();

  if( pid_pipe== 0){
    close(fd[0]);
    dup2(fd[1],1);
    redirections_or_command(processes_in_pipes[0]);
    execute_comand_name(processes_in_pipes[0]);
  }

  else {
    close(fd[1]);
    wait(&pid_pipe);

    pipe(temp_fd);

    int pid_pipe= fork();

    if(pid_pipe==0){
      close(temp_fd[0]);
      dup2(temp_fd[1],1);
      dup2(fd[0],0);
      redirections_or_command(processes_in_pipes[1]);
      execute_comand_name(processes_in_pipes[1]);
    }

    else{
      wait(&pid_pipe);
      close(temp_fd[1]);
      dup2(temp_fd[0],0);
      //dup2(1,1);
      //cout<<"here we are \n";

    }

  }////fork() else{}
  //string test_input ;
  //getline(cin,test_input);
  //cout<<"testing some things here "<<test_input<<endl ;

  ////////begin multi-piping /////////////
  for(int i =2; i< processes_in_pipes.size(); i++){

    if(i= processes_in_pipes.size()-1){
      //close(new_fds[0]);
      //dup2(new_fds[1],1);
      redirections_or_command(processes_in_pipes[i]);
      execute_comand_name(processes_in_pipes[i]);
    }

    int new_fds[2];
    pipe(new_fds);

    int future_pid = fork();

    if( future_pid==0){
      close(new_fds[0]);
      dup2(new_fds[1],1);
      redirections_or_command(processes_in_pipes[i]);
      execute_comand_name(processes_in_pipes[i]);
    }
    else{
      wait(&future_pid);
      close(new_fds[1]);
      dup2(new_fds[0],0);
    }

  }
  return ;
}

std::vector<string > get_tokens(string line ){
  std::vector<string> tokens;

  // stringstream class check1
  stringstream check1(line);

  string temp_token;

  // Tokenizing w.r.t. space ' '
  while(getline(check1,temp_token, ' '))
  {
      tokens.push_back(temp_token);
      int next_val = check1.peek();
      char character_ = '0'+ next_val;
      if(next_val== '\"'){
        check1.get();
        getline(check1,temp_token,'\"');

        tokens.push_back(temp_token);
        //cout<<"***************-"<<temp_token<<"-***********"<<endl;
      }

  }

  return tokens;
}

void redirections_or_command(std::vector<string > cmd_line){
  if(redirection(cmd_line) ){
    execute_redirection(cmd_line);
  }
  else{
    execute_comand_name(cmd_line);
  }

  return;
}


void execute_one_unix_command(std::vector<string > temp_cmdline,int & exit){

  if(special_command(temp_cmdline) ){
    chdir(temp_cmdline[1].c_str());

  }

  else{
  int pid = fork();


  if(pid==0){

  //for(int i =0; i< programs_list[j].size(); i++){

    if( command_name(temp_cmdline) ){ ///command name with ARGS
      execute_comand_name(temp_cmdline);
    }

    else if( pipe_command(temp_cmdline) ){
      execute_pipe_command(temp_cmdline);
    }/// pipe commands

    else if( redirection(temp_cmdline) ){ // redirection command with ARGS
      execute_redirection(temp_cmdline);
    }
    else if(special_command(temp_cmdline) ){
      exit++;


    }

    else if(temp_cmdline[0]=="exit"){ /// EXIT special command
      cout<<"Exiting Shell Program....\n";
      exit++;

    }

    else{//// Invalid Syntax/// wont //close shell
      cout<<"invalid argument "<<endl ;
      exit++;

    }

  //}//for loop
}



else{////////////////////parent side of the code/////////////
//cout<<"waiting for child \n";

wait(&pid);
//execute_special_command(temp_cmdline);
//cout<<"back to shell program "<<endl;
if(temp_cmdline[0]=="exit"){
  //cout<<"exiting....\n"
  exit++;
}

}////end of parent


}

  return;
}
