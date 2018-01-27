#include <nds.h>

#include "App.h"
#include "EmuScreen.h"
#include "Kbd.h"
#include "Cpu.h"

#include "rom_bin.h"

App APP;

void App::init() {

	vramSetBankA(VRAM_A_MAIN_BG); // 128 kB at base of main background memory
	lcdMainOnTop();
	powerOn(POWER_ALL_2D);
	EmuScreen::set_main();

	KBD.init();
	CPU.init();

	for (size_t i = 0; i < rom_bin_size / 2; i++) {
		CPU.rom[i] = ((u16 *)rom_bin)[i];
	}

	CPU.setfreq(1000000); // as of yet unachievable target IPS

	console.initialise(CONSOLE_SUB_H);
	console.activate();
}

void App::tick() {

	cpuStartTiming(0);

	KBD.tick();
	CPU.tick();

	cpuEndTiming();
	tick_cpu_time = (tick_cpu_time * 3 + cpuGetTiming()) >> 2;
}

void App::draw() {

	cpuStartTiming(0);

	EmuScreen::draw();

	cpuEndTiming();
	draw_cpu_time = (draw_cpu_time * 3 + cpuGetTiming()) >> 2;

	console.clear();
	console << "Emulator CPU: " << CONSOLE_CYAN << (tick_cpu_time * 100 / 560190) << "%\n" << CONSOLE_WHITE;
	console << "Renderer CPU: " << CONSOLE_CYAN << (draw_cpu_time * 100 / 560190) << "%\n" << CONSOLE_WHITE;
}
