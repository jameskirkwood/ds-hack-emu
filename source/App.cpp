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

	CPU.reset();
	CPU.set_ips_cap(2000000); // 2MHz

	// for (size_t i = 0; i < rom_bin_size / 2; i++) {
	// 	CPU.rom[i] = ((u16 *)rom_bin)[i];
	// } // This method appears to corrupt EmuScreen::vram ... Why?
	dmaCopy(rom_bin, CPU.rom, rom_bin_size);

	console.initialise(CONSOLE_SUB_H);
	console.activate();
}

void App::tick() {

	scanKeys();

	if (keysDown() & KEY_START) CPU.reset();

	CPU.tick();
}

void App::draw() {

	cpuStartTiming(0);
	EmuScreen::draw();
	cpuEndTiming();
	draw_cpu_time = (draw_cpu_time * 3 + cpuGetTiming()) >> 2;

	console.clear();
	console << "Renderer CPU: " << CONSOLE_CYAN << (draw_cpu_time * 100 / 560190) << "%\n" << CONSOLE_WHITE;
	console << "IPS: " << CONSOLE_CYAN << CPU.get_ips() << "\n" << CONSOLE_WHITE;
}

void App::run() {

	CPU.run();
}
