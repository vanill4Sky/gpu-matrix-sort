#include "timer.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std::chrono;

void gms::timer::start() 
{
	sta = 0;
	sto = 0;
	sta = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

void gms::timer::stop() 
{
	sto = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

unsigned long long gms::timer::get_time() 
{
	return sto - sta;
}

//ZWRACA WYNIK JAKO STRING MINUTY:SEKUNDY:MILISEKUNDY:NANOSEKUNDY
std::string gms::timer::get_time_as_string() 
{
	std::stringstream ss;

	const auto temp = get_time();
	const auto milisec = temp / 1000000;
	const auto sec = milisec / 1000;
	const auto min = sec / 60;

	ss << " m: s: ms:    ns\n";
	ss << std::setfill(' ');
	ss << std::setw(2) << std::to_string(min) << ":";
	ss << std::setw(2) << std::to_string(sec % 60) << ":";
	ss << std::setw(3) << std::to_string(milisec % 1000) << ":";
	ss << std::setw(6) << std::to_string(temp % 1000000);

	return ss.str();
}
