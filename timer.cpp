#include "timer.h"
#include <iostream>
using namespace std;

timer* timer:: sinstance = NULL;

timer* timer :: instance() {
	if (sinstance == NULL)
		sinstance = new timer();
	return sinstance;
}

void timer::release() {
	delete sinstance;
	sinstance = NULL;
}

timer::timer() {
	reset();
	mtimescale = 1.0f;
}

timer :: ~timer() {

}

void timer::reset() {
	mstartticks = SDL_GetTicks();
	melapsedticks = 0;
	mdelatatime = 0.0f;
}

float timer::deltatime() {
	return mdelatatime;
}

void timer::timescale(float t) {
	mtimescale = t;
}

float timer :: timescale() {
	return mtimescale;
}

void timer::update() {
	melapsedticks = SDL_GetTicks() - mstartticks;
	mdelatatime = melapsedticks + 0.001f;
}
