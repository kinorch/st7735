#include <iostream>
#include <stdint.h>

#include "mraa.hpp"

#include "st7735.h"

using namespace std;

int main() {
	cout << "[SPI_TEST] enter" << endl;
	cout << "MRAA Ver " + mraa::getVersion() << endl;

	st7735* lcd = new st7735();
	lcd->Initialize(1,14,15);

	color18bit_t color;
	for(volatile int i=0x00; i<=0xFF; i+=0xFF)
	{
		for(volatile int j=0x00; j<=0xFF; j+=0xFF)
		{
			for(volatile int k=0x00; k<=0xFF; k+=0xFF)
			{
				color.r = i;
				color.g = j;
				color.b = k;
				lcd->Clear(&color);
			}
		}
	}
	color.r = 0x00;
	color.g = 0x00;
	color.b = 0x00;
	lcd->Clear(&color);
	delete lcd;

	cout << "[SPI_TEST] return" << endl;
	return 0;
}
