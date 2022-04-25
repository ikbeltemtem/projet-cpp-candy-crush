
#pragma once
#include <iostream>
#include <SDL.h>
using namespace std;

class timer
{

public :

	static timer* sinstance;
	unsigned int mstartticks;
	unsigned int melapsedticks;
	float mdelatatime;
	float mtimescale;

public:

	static timer* instance();
	static void release();

	void reset();
	float deltatime();

	void timescale(float t);
	float timescale();

	void update();

private:

	timer();
	~timer();
};
