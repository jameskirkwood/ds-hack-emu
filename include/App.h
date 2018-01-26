#ifndef APP_H
#define APP_H

#include "Console.h"

class App {

private:

	Console console;

public:

	void init();
	void tick();
	void draw();

	int draw_cpu_time;
	int tick_cpu_time;
};

extern App APP;

#endif
