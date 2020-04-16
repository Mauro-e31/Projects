#include<iostream>
#include<vector>
#include<stdio.h>

using namespace std;

int main(int argc, char const *argv[])
{
    cout<<"hello world"<<endl;

    return 0;
}

/*
for(int j =0; j<processes_in_pipes.size(); j++){
  cout<<"j= "<<j<<endl;

  int fd[2];
  int pid2=1;

  pipe(fd);
  cout<<"argument line = ";print_vector_string(processes_in_pipes[j]);
  cout<<"fd[0]= "<<fd[0]<<endl ;
  cout<<"fd[1]= "<<fd[1]<<endl ;
  if( processes_in_pipes.size()-1 !=j ){
  //cout<<"test 2 \n";
  //cout<<"processes_in_pipes.size()-1 = "<<processes_in_pipes.size()-1<< " j = "<<j<<endl;
  pid2=fork();
}

  if( pid2==0){
    //cout<<"executing command name = "<<processes_in_pipes[j][0]<<endl;
    cout<<"hello from child"<<endl;
    cout<<"j = "<<j<<endl;
    //cout<<"argument line = ";print_vector_string(processes_in_pipes[j]);
    if( j==0 ){
      close(fd[0]);
      dup2(fd[1],1);
      execute_comand_name(processes_in_pipes[j]);
    }

    else{
      cout<<"temp_fd[0]= "<<temp_fd[0]<<endl ;
      cout<<"temp_fd[1]= "<<temp_fd[1]<<endl ;
      cout<<"fd[0]= "<<fd[0]<<endl ;
      cout<<"fd[1]= "<<fd[1]<<endl ;
      cout<<"Child outputing to "<<fd[0]<<endl ;
      cout<<"Inputs from = "<< temp_fd[0]<<endl<<endl;
      cout<<endl;
      dup2(fd[1],1);
      dup2(temp_fd[0],0);
      close(fd[0]);
      execute_comand_name(processes_in_pipes[j]);
      //close(fd[0]);
      /*
      int test_here = fork();
      if(test_here==0){
        dup2(temp_fd[0],0);
        close(fd[0]);
        execute_comand_name(processes_in_pipes[j]);
      }

      else{
        wait(&test_here);
        dup2(fd[1],1);
        dup2(temp_fd[0],0);
        close(fd[0]);
        execute_comand_name(processes_in_pipes[j]);
      }

    }


  }

  else{
    wait(&pid2);
    cout<<"hello from parent "<<endl;
    //cout<<"fd[0]= "<<temp_fd[0]<<endl ;
    //cout<<"fd[1]= "<<fd[1]<<endl ;
    if( processes_in_pipes.size()-1==j ){
    cout<<"temp_fd[0]= "<<temp_fd[0]<<endl ;
    cout<<"temp_fd[1]= "<<temp_fd[1]<<endl ;
    close(fd[1]);
    close(fd[0]);
    dup2(temp_fd[0],0);
    cout<<"chilling"<<endl;
    execute_comand_name(processes_in_pipes[j]);
    //string test_99;
    //getline(cin,test_99);
    //cout<<test_99<<endl;
    exit++;
    break;
  }

  cout<<"after parent \n\n"<<endl;

}






temp_fd[0]= fd[0];
temp_fd[1]= fd[1];

/*
int pid2 = fork();
if( pid2==0){
  //close(fd[0]);
  dup2(fd[1],1);
  execute_comand_name()

}
}///for looop
*/
