#ifndef KBD_H
#define KBD_H

#include <nds.h>

class Kbd {
	
private:

public:

	void init();
	void tick();

	u16 read();
};

extern Kbd KBD;

#endif
