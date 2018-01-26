#include <iostream>
#include <iomanip>

int main() {
	uint16_t data;
	while (std::cin.peek(), !std::cin.eof()) {
		data = 0;
		for (int i = 0; i < 16; i++) {
			data <<= 1;
			if (std::cin.get() == '1') data++;
		}
		std::cin.ignore(2, '\n');
		std::cout << char(data & 0xff) << char(data >> 8);
	}
}