#ifndef MY_SHELL_H
#define MY_SHELL_H
#include<unistd.h>
#include<iostream>
#include<sys/wait.h>
#include<string>
#include<vector>
//#include<dirent.h>
using namespace std; 

string get_current_dir();

string get_args_to_string(vector<string> );

#endif