#include <stdio.h>
#include <dos.h>

int main() {
	char far *PC_type = (char far *)0xF000FFFE;
	char diagnostic_byte;

	printf("Microprocessor type: i don't know.\n");

	printf("PC type: '%x'\n", (char)(*PC_type));

	outport(0x70, 0x0E);
	diagnostic_byte = inportb(0x71);
	printf("Battery status: %d\n", diagnostic_byte & 0x8);

	getch();

	return 0;
}
