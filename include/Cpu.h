#ifndef CPU_H
#define CPU_H

#include <nds.h>

class Cpu {
	
private:

	volatile int64 mibicycles_per_tick;
	volatile int64 mibicycles_due;
	volatile int64 mibicycles_planned;
	volatile int64 mibicycles_last_tick;

public:

// Application interface
//-----------------------------------------------------------------------------

	// Call 60 times per second
	void tick();

	// Execute a bounded but indeterminate number of cycles
	void run();

	// Set a limit on the average number of cycles (instructions) per second
	void set_ips_cap(int ips) { mibicycles_per_tick = (ips << 10) / 60; }

	// Get the rate of cycles (instructions) per second during the last tick
	int get_ips() { return (mibicycles_last_tick >> 10) * 60; }

// Virtual hardware interface
//-----------------------------------------------------------------------------

	// Reset CPU
	void reset() { pc = ra = rd = 0; }

	u16 pc, ra, rd;

	u16 ram[24 << 10];
	u16 rom[64 << 10];
};

extern Cpu CPU;

#endif
