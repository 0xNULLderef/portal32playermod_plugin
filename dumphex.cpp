#include <dumphex.hpp>

void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for(i = 0; i < size; ++i) {
		console->Print("%02X ", ((unsigned char*)data)[i]);
		if(((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if((i+1) % 8 == 0 || i+1 == size) {
			console->Print(" ");
			if((i+1) % 16 == 0) {
				console->Print("|  %s \n", ascii);
			} else if(i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if((i+1) % 16 <= 8) {
					console->Print(" ");
				}
				for(j = (i+1) % 16; j < 16; ++j) {
					console->Print("   ");
				}
				console->Print("|  %s \n", ascii);
			}
		}
	}
}