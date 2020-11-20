#pragma once

#include <string>

namespace gms
{

class timer
{
public:
	void start();
	void stop();
	unsigned long long get_time();
	std::string get_time_as_string();

private:
	unsigned long long sta;
	unsigned long long sto;
};

} 
