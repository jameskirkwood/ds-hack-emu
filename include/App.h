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

	void run();

	int draw_cpu_time;
	int cpu_ips;
};

extern App APP;

#endif
