// CON_LANG.cpp : Defines the entry point for the console application.
//
//0x0A - start print, followed by printable
//0x0B - concludes printable

//0x00 - null
//0x01 - string identifier
//0x02 - integer identifier
//0x03 - boolean identifier

#include "stdafx.h"
#include "connamespace.h"

#include <iostream>
#include <fstream>
#include <io.h>
#include <exception>
#include <vector>

int main(int argv, char **argc)
{
	if (argc[1] == NULL) { return 1; }
	con::compile(argc[1]);
	std::cin.ignore();
	return 0;
}

namespace con {

	string WRITE_LIST = "sal "; //What is written to the file. Initalized to start 256 bytes into the segment.

	//Used in the compiled file to save space and refer to variables.
	vector<string> variable_names = {};
	vector<int> variable_ids = {};

	//Called by main to compile.
	void compile(string filename) {
		ifstream file;
		file.open(filename);
		if (!file || !filename.find(".con")) {
			con::throwError("FileNotFoundException", "Unable to load file: " + filename + "!"); //Errors when a file is not found.
			exit(EXIT_FAILURE);
		}

		int len = filename.size();

		string pure_filename = filename.substr(0, len - 4);
		int line_num = 1;
		string line;
		while(getline(file, line)) {
			
			if(line[0] + line[1] == (char)"//"){
				continue;
			}
			
			try {
				var_construct(line, line_num);
			}
			catch (exception) {
				con::throwError("VariableSyntaxException", "Unable to parse line " + to_string(line_num)  + ".");
			}
			
			try {
				con::print_construct(line, line_num);
			}
			catch (exception) {
				con::throwError("PrintSyntaxException", "Unable to parse line " + to_string(line_num)  + ".");
			}
			line_num = line_num + 1;
		}
		ofstream compiled_file; //The new compilation file.
		compiled_file.open(pure_filename + ".cdl", ios::out);
		compiled_file << WRITE_LIST.c_str();
		compiled_file.close();
	}





	//VARIABLE CONSTRUCT
	void var_construct(string list, int line_num) {
		
		//Checks syntax.

		bool shouldDelete = true;
		int j = 0;
		int startWhere = 0;
		while (shouldDelete == true) { //Removes indentations.
			if (list[j] != ' ') {
				shouldDelete = false;
				startWhere = j;
			}
			j = j + 1;
		}
		vector<char> varname = {}; //Variable names can be only 255 characters long.
		char temp[4];
		temp[0] = list[startWhere];
		temp[1] = list[startWhere + 1];
		temp[2] = list[startWhere + 2];
		temp[3] = NULL;

		if (temp == "var") { //Checks for the keyword "var".
			if (list[startWhere + 3] == ' ' && list.find(" equals")) {
				startWhere = startWhere + 4;
				bool continueLoop = true;
				int i = 0;
				int k = startWhere;
				char temp2[8];
				while (continueLoop == true) {
					temp2[0] = list[k];
					temp2[1] = list[k + 1];
					temp2[2] = list[k + 2];
					temp2[3] = list[k + 3];
					temp2[4] = list[k + 4];
					temp2[5] = list[k + 5];
					temp2[6] = list[k + 6];
					temp2[7] = NULL;

					string checker = " equals";
					if (temp2 != checker) { //Makes sure that the equals is not part of the varname
						if (list[k] == ' ') {
							throwError("VariableSyntaxException", "A variable at line " + to_string(line_num) + " contains a space!");
						}
						try {
							varname.push_back(list[k]);
							i = i + 1;
						}
						catch (out_of_range) {
							throwError("VariableSyntaxException", "Variable name too long at line " + to_string(line_num) + '.');
						}
					}
					else {
						continueLoop = false;
						varname.push_back(NULL);
						startWhere = k + 7;
						break;
					}
					k = k + 1;
				}
			}
			else {
				throwError("VariableSyntaxException", "Failed to parse line " + to_string(line_num) + '.');
			}

			//Assigns values.

			if (list[startWhere] != ' ') {
				throwError("VariableSyntaxException", "Failed to parse line " + to_string(line_num) + '.');
			}

			string markType = "";
			vector<char> value = {};
			startWhere = startWhere + 1;
			
			char temp3[8];

			if (list[startWhere] == ' ' || list[startWhere] == ';' || list[startWhere] == NULL) {
				throwError("VariableSyntaxException", "Failed to parse line " + to_string(line_num) + '.');
			}
			if (list[startWhere + 1] != ';' && list[startWhere + 2] != ';' && list[startWhere + 3] != ';'){ //Used for when the value IS a boolean.
				temp3[0] = list[startWhere];
				temp3[1] = list[startWhere + 1];
				temp3[2] = list[startWhere + 2];
				temp3[3] = list[startWhere + 3];

				if (list[startWhere + 4] == ';') {
					temp3[4] = NULL;
				}
				else if (list[startWhere + 4] == ' ') {
					throwError("VariableSyntaxException", "Failed to parse line " + to_string(line_num) + '.');
				}
				else {
					temp3[4] = list[startWhere + 4];
					temp3[5] = NULL;
				}
			}

			char temp4[2];
			temp4[0] = '\'';
			temp4[1] = NULL;
			if (list[startWhere] == '"') {
				markType = "string";
				if (list.find(startWhere + 1, '"')) {
					bool continueLoopA = true;
					int z = startWhere + 1;
					while (continueLoopA == true) {
						if (list[z] == '"') {
							continueLoopA = false;
							value.push_back(NULL);
							break;
						}
						value.push_back(list[z]);
						z = z + 1;
					}
				}
			}
			else if (list[startWhere] == '\'') {
				markType = "string";
				if (list.find(startWhere + 1, '\'')) {
					bool continueLoopA = true;
					int z = startWhere + 1;
					while (continueLoopA == true) {
						if (list[z] == '\'') {
							continueLoopA = false;
							value.push_back(NULL);
							break;
						}
						value.push_back(list[z]);
						z = z + 1;
					}
				}
			}

			else if (isdigit(list[startWhere])) {
				markType = "int";
				bool continueLoopA = true;
				int z = startWhere;
				while (continueLoopA == true) {
					if (list[z] == ';') {
						continueLoopA = false;
						value.push_back(NULL);
						break;
					}
					value.push_back(list[z] + NULL);
					z = z + 1;
				}
			}
			else if (strcmp(temp3, "true") || strcmp(temp3, "false")) {
				markType = "bool";
				bool continueLoopA = true;
				int z = 0;
				for (z; z <= 8; z++) {
					value.push_back(temp3[z]);
				}
				value.push_back(NULL);
			}

			else {
				throwError("VariableSyntaxException", "No value assigned to a variable at line " + to_string(line_num) + '.');
			}

			//TODO: Create memory allocations from variable names.
			WRITE_LIST.append(varname.data()); //TODO: CREATE WRITER!
			WRITE_LIST.append(" ");
			WRITE_LIST.append(value.data());
		}
	}





	//PRINT CONSTRUCT
	void print_construct(string list, int line_num){
		bool shouldDelete = true;
		int j = 0;
		int startWhere = 0;
		while (shouldDelete == true) { //Removes indentations.
			if (list[j] == (char)" " && shouldDelete == true) {
				list[j] = NULL;
			}
			j = j + 1;
			if (list[j] != ' ') {
				shouldDelete = false;
				startWhere = j;
			}
		}

		startWhere = startWhere - 1;
		char temp[5];
		vector<char> temp2 = {};
		temp[0] = list[startWhere];
		temp[1] = list[startWhere + 1];
		temp[2] = list[startWhere + 2];
		temp[3] = list[startWhere + 3];
		temp[4] = list[startWhere + 4];

		char temp99[2];
		temp99[0] = list[startWhere + 5];
		temp99[1] = list[startWhere + 6];

		if (strcmp(temp, "print")) { //Checks for the keyword "print".
			if (strcmp(temp99, "('") && list.find("')")) {
				startWhere = startWhere + 7;
				bool continueLoop = true;
				int k = 0;
				while (continueLoop == true) {
					cout << "A";
					if (k >= startWhere) {
						if (list[k] == '\'') {
							if (list[k + 1] != ')') {
								throwError("PrintSyntaxException", "Failed to parse line " + to_string(line_num) + '.');
							}
							continueLoop = false;
						}
						temp2.push_back(list[k]);
					}
					k = k + 1;
				}
			} //TODO: Add else-if to print variable values.
			else if (strcmp(temp99, "(\"") && list.find('")')) {
				startWhere = startWhere + 7;
				bool continueLoop = true;
				int k = 0;
				while (continueLoop == true) {
					if (k >= startWhere) {
						if (list[k] == '"') {
							if (list[k + 1] != ')') {
								throwError("PrintSyntaxException", "Failed to parse line " + to_string(line_num) + '.');
							}
							continueLoop = false;
						}
						temp2.push_back(list[k]);
						k = k + 1;
					}
				}
			}
			else {
				throwError("PrintSyntaxException", "Failed to parse line " + to_string(line_num) + '.');
			}
			WRITE_LIST += (string)"0A'" += (string)temp2.data() += (string)"'0B";
		}
	}
	
	void throwError(string err, string msg) { //Throws an error.
		cout << "A fatal exception has occured! Compilation can not continue!" << endl;
		cout << "Exception Type: " + err + "." << endl;
		cout << msg;
		exit(EXIT_FAILURE);
	}
}