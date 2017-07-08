#pragma once

#include "stdafx.h";

namespace con {
	void compile(string filename);

	void var_construct(string line, int line_num);

	void throwError(string err, string msg);
}