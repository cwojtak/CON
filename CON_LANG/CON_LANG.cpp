// CON_LANG.cpp : Defines the entry point for the console application.
//
//0x0A - start print, followed by printable
//0x0B - concludes printable

#include "stdafx.h"
#include "connamespace.h"

#include <iostream>
#include <fstream>
#include <io.h>
#include <exception>

int main(int argv, char **argc)
{
	if (argc[1] == NULL) { return 1; }
	con::compile(argc[1]);
	std::cin.ignore();
	return 0;
}

namespace con {

	string WRITE_LIST = "sal "; //Initalized to start 256 bytes into the segment

	void compile(string filename) {
		ifstream file;
		file.open(filename);
		if (!file || !filename.find(".con")) {
			throwError("FileNotFoundException", "Unable to load file: " + filename + "!"); //Errors when a file is not found.
			exit(EXIT_FAILURE);
		}

		int len = filename.size();

		string pure_filename = filename.substr(0, len - 4);
		int line_num = 1;
		string line;
		while(getline(file, line)) {
			
			if(line[1] += line[2] == "//"){
				continue;
			}
			
			try {
				var_construct(line, line_num);
			}
			catch (exception) {
				throwError("VariableSyntaxError", "Unable to parse line " + std::to_string(line_num)  + ".");
			}
			
			try {
				print_construct(line, line_num);
			}
			catch (exception) {
				throwError("PrintSyntaxError", "Unable to parse line " + std::to_string(line_num)  + ".");
			}
			line_num = line_num + 1;
		}
		ofstream compiled_file; //The new compilation file.
		compiled_file.open(pure_filename + ".cdl", ios::out);
		compiled_file << WRITE_LIST.c_str();
		compiled_file.close();
	}

	void var_construct(string line, int line_num) { //Checks to see if there are any variable definitions.
		char list[65535];
		int i = 0;
		while(i != line.length()) { //Inserts all the characters of the line into a list.
			list[i] = line[i];
			i = i + 1;
		}

		bool shouldDelete = true;
		int j = 0;
		int startWhere = 0;
		while (shouldDelete == true) { //Removes indentations.
			if (list[j] == (char)" " && shouldDelete == true) {
				list[j] = NULL;
			}
			j = j + 1;
			if (list[i] != (char)" " || j == 65534) {
				shouldDelete = false;
				startWhere = j;
			}
		}

		startWhere = startWhere - 1;
		string temp = "";
		string temp3 = "";
		cout << list[startWhere];
		temp += list[startWhere];
		cout << "derp derp depr";
		temp += list[startWhere + 1];
		temp += list[startWhere + 2];

		if(temp == "var") { //Checks for the keyword "var".
			if(list[startWhere + 3] == (char)" " && line.find(" equals")) {
				startWhere = startWhere + 4;
				bool continueLoop = true;
				int k = 0;
				string temp2 = "";
				temp3 = "";
				while (continueLoop == true) {
					if (k >= startWhere) {
						temp2 += list[k];
						temp2 += list[k + 1];
						temp2 += list[k + 2];
						temp2 += list[k + 3];
						temp2 += list[k + 4];
						temp2 += list[k + 5];
						temp2 += list[k + 6];
						
						if (temp2 != (char*)" equals") { //Makes sure that the equals is not part of the varname
							if(list[startWhere + k] == " "){
								throwError("VariableSyntaxError", "A variable at line " + line_num + " contains a space!")
							}
							
							temp3 += list[startWhere + k];
						}
						else {
							continueLoop = false;
						}
						k = k + 1;
					}
				}
			}
			else {
				throwError("VariableSyntaxError", "Failed to parse line " + line_num + ".");
			}
		}
		
		WRITE_LIST += temp3 + " "; //TODO: CREATE WRITER!
		cout << list[startWhere] + list[startWhere + 1] + list[startWhere + 2];
	}

	void print_construct(string line, int line_num){
		char list[65535];
		int i = 0;
		while(i != line.length()) { //Inserts all the characters of the line into a list.
			list[i] = line[i];
			i = i + 1;
		}
		
		bool shouldDelete = true;
		int j = 0;
		int startWhere = 0;
		while (shouldDelete == true) { //Removes indentations.
			if (list[j] == (char)" " && shouldDelete == true) {
				list[j] = NULL;
			}
			j = j + 1;
			if (list[i] != (char)" " || j == 65534) {
				shouldDelete = false;
				startWhere = j;
			}
		}
		
		startWhere = startWhere - 1;
		string temp = "";
		string temp2 = "";
		cout << list[startWhere];
		temp += list[startWhere];
		cout << "derp derp depra";
		temp += list[startWhere + 1];
		temp += list[startWhere + 2];
		temp += list[startWhere + 3];
		temp += list[startWhere + 4];
		
		if(temp == "print") { //Checks for the keyword "var".
			if(list[startWhere + 5] += list[startWhere + 6] == "('" and line.find("')")) {
				startWhere = startWhere + 7;
				bool continueLoop = true;
				int k = 0;
				while (continueLoop == true) {
					if (k >= startWhere) {
						if(list[k] == "'"){
							if(list[k + 1] != ")"){
								throwError("PrintSyntaxError", "Failed to parse line " + line_num + ".")
							}
							continueLoop = false;
						}
						temp2 += list[k]
						k = k + 1;
					}
				}
			} //TODO: Add else-if to print variable values.
			else if(list[startWhere + 5] += list[startWhere + 6] == '("' and line.find('")')){
				startWhere = startWhere + 7;
				bool continueLoop = true;
				int k = 0;
				while (continueLoop == true) {
					if (k >= startWhere) {
						if(list[k] == '"'){
							if(list[k + 1] != ")"){
								throwError("PrintSyntaxError", "Failed to parse line " + line_num + ".")
							}
							continueLoop = false;
						}
						temp2 += list[k]
						k = k + 1;
					}
				}
			}
			else {
				throwError("PrintSyntaxError", "Failed to parse line " + line_num + ".");
			}
		}
		
		WRITE_LIST += "0x0A'" += temp2 += "'0x0B";	
	}
	
	void throwError(string err, string msg) { //Throws an error.
		cout << "A fatal exception has occured! Compilation can not continue!";
		cout << "Exception Type: " + err + ".";
		cout << msg;
		exit(EXIT_FAILURE);
	}
}