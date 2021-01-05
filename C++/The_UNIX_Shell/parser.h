#ifndef PARSER_H
#define PARSER_H

#include<vector>
#include<iostream>
#include<string>
#include<sstream>

using namespace std; 

vector<string> get_tokens(string); 

void print_cmd_tokens(vector<string>); 

vector<vector<string>> get_cmds_with_args(vector<string>); 

bool special_operator(string);

void print_cmd_with_args(vector<vector<string>>); 

///Parse data and return commands with arguments/flags
vector<vector<string>>parse_cmd_line();
#endif