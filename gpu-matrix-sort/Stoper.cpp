#include <iostream>
#include "Stoper.h"
#include <chrono>

using namespace std::chrono;

long long sta = 0;
long long sto = 0;

//STARTUJE ZEGAR
void Stoper::start() {
	sta = 0;
	sto = 0;
	sta = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

//ZATRZYMUJE ZEGAR
void Stoper::stop() {
	sto = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

//ZWRACA WYNIK
unsigned long long Stoper::getTime() {
	using namespace std;
	return sto - sta;
}
//ZWRACA WYNIK JAKO STRING MINUTY:SEKUNDY:MILISEKUNDY:NANOSEKUNDY
string Stoper::getTimeString() {
	string resoult;
	unsigned long long temp = getTime();

	int milisec = (temp/1000000);
	int sec = (milisec/1000);
	int min = sec/60;
	string nanosecS = to_string(temp % 1000000);
	while (nanosecS.length() < 6)
		nanosecS = " " + nanosecS;
	string milisecS = to_string(milisec % 1000);
	while (milisecS.length() < 3)
		milisecS = " " + milisecS;
	string secS = to_string(sec % 60);
	while (secS.length() < 2)
		secS = " " + secS;
	string minS = to_string(min);
	while (minS.length() < 2)
		minS = " " + minS;

	resoult = " m: s: ms:    ns\n"+minS+":"+secS+":"+milisecS+":"+nanosecS;

	return resoult;
}
