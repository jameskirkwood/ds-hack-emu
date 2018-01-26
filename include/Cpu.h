#ifndef CPU_H
#define CPU_H

#include <nds.h>

class Cpu {
	
private:

	int cycles_due;
	int cycles_per_frame;

public:

	void init();
	void tick();

	void setfreq(int);

	u16 pc, ra, rd;

	u16 ram[24 << 10];
	u16 rom[64 << 10];
};

extern Cpu CPU;

#endif
