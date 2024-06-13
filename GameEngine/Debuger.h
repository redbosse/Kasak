#pragma once
#include <iostream>
#include "Object.h"

using namespace std;

class Debuger
{
public:

	Debuger() {
		cout << "\n";
	}

	Debuger(const char* debug_log) {
		cout << debug_log << "\n";
	}
};

template<typename T>
static void Debug_Log(T debug_log) {
	cout << debug_log << "\n";
}
