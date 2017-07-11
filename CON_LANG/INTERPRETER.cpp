// CON_LANG.cpp : Defines the entry point for the console application.
//

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
	
	void compile(string filename) {
		ifstream file;
		file.open(filename);
		if (!file || !filename.find(".cdl")) {
			throwError("FileNotFoundException", "Unable to load file: " + filename + "!"); //Errors when a file is not found.
			exit(EXIT_FAILURE);
		}

		int len = filename.size();
		int line_num = 1;
		string line;
		while(getline(file, line)) {
			
			char list[65535];
			int i = 0;
			while(i != line.length()) { //Inserts all the characters of the line into a list.
				list[i] = line[i];
				i = i + 1;
			}
			
			int startNext = 0;
			bool continueLoop = true;
			while(continueLoop == true){
				//try {
					//var_construct(line, line_num);
				//}
				//catch (exception) {
					//throwError("VariableSyntaxError", "Unable to parse line " + std::to_string(line_num)  + ".");
				//}
				
				try {
					if(list[startNext] += list[startNext + 1] += list[startNext + 2] += list[startNext + 3] == "0x0A"){
						startNext = print_construct(line, startNext + 5);
						continue;
					}
				}
				
				catch (exception) {
					throwError();
				}
			}
			line_num = line_num + 1;
		}
	}

	void var_construct(string line, int line_num) { //Checks to see if there are any variable definitions.
	}

	int print_construct(char list[], int start){
		continueLoop = false;
		string echo = "";
		bool continueLoop2 = true;
		int j = start;
		while(continueLoop2 == true){
			if(list[j] == "'"){
				continueLoop2 = false;
				break;
			}
			echo += list[j];
			j = j + 1;
		}
		cout << echo;
		return j + 4;
	}
	
	void throwError() { //Throws an error.
		cout << "A fatal exception has occured! This library is corrupted! Try rebuilding the library.";
		exit(EXIT_FAILURE);
	}
}