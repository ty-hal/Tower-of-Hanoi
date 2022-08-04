#pragma once
#include <iostream>
#include <chrono>
using namespace std;
using highResClock = chrono::high_resolution_clock;
using timePoint = highResClock::time_point;

class SimpleTimer
{
public:
	SimpleTimer(const char* message) {
		this->message = message;
		start = highResClock::now();
	}
	
	chrono::duration<double> GetTime() {
		return (highResClock::now() - start);
	}

	void ResetTime() {
		start = highResClock::now();
	}

	~SimpleTimer() {
		end = highResClock::now();
		elapsed = end - start;
		cout << message << " elapsed time: " << elapsed.count() << endl;
	}
private:
	timePoint start;
	timePoint end;
	chrono::duration<double> elapsed;
	string message;
};

