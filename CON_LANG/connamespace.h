#pragma once

#include "stdafx.h";

namespace con {
	void compile(string filename);

	void var_construct(string line, int line_num);

	void print_construct(string line, int line_num);

	void throwError(string err, string msg);

	void read(string filename);

	void var_constructb(string line, int line_num);

	int print_constructb(string list, int start);

	void throwError();
}