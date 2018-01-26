#ifndef VID_H
#define VID_H

class Vid {
	
private:

	u16 vram_old[8 << 10];

	int bg_id;

public:

	void init();
	void draw();

	int frame_strips;
};

extern Vid VID;

#endif
