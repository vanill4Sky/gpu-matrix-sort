#pragma once
//#include <iostream>
#include <sstream>

using namespace std;

class Stoper
{
private:
	unsigned long long sta;
	unsigned long long sto;
public:

	void start();
	void stop();
	unsigned long long getTime();
	string getTimeString();
}; 
