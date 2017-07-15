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

	string WRITE_LIST;

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
			try {
				var_construct(line, line_num);
			}
			catch (exception) {
				throwError("VariableSyntaxError", "Unable to parse line " + std::to_string(line_num)  + ".");
				line_num = line_num + 1;
			}
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
		temp.append((char*)list[startWhere]);
		cout << "derp derp depr";
		temp.append((char*)list[startWhere + 1]);
		temp.append((char*)list[startWhere + 2]);

		if(temp == (char*)"var") { //Checks for the keyword "var".
			if(list[startWhere + 3] == (char)" " && line.find(" equals")) {
				bool continueLoop = true;
				int k = 0;
				string temp2 = "";
				temp3 = "";
				while (continueLoop == true) {
					if (k >= startWhere) {
						temp2.append((char*)list[k]);
						temp2.append((char*)list[k + 1]);
						temp2.append((char*)list[k + 2]);
						temp2.append((char*)list[k + 3]);
						temp2.append((char*)list[k + 4]);
						temp2.append((char*)list[k + 5]);
						temp2.append((char*)list[k + 6]);
						
						if (temp2 != (char*)" equals") { //Makes sure that the equals is not part of the varname
							temp3.append((char*)list[startWhere]);
						}
						else {
							continueLoop = false;
						}
						k = k + 1;
					}
				}
			}
			else {
				return;
			}
		}
		WRITE_LIST = temp3;
		cout << list[startWhere] + list[startWhere + 1] + list[startWhere + 2];
	}

	void throwError(string err, string msg) { //Throws an error.
		cout << "A fatal exception has occured! Compilation can not continue!";
		cout << "Exception Type: " + err + ".";
		cout << msg;
		exit(EXIT_FAILURE);
	}
}