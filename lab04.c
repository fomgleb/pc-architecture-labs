#include <dos.h>
#include <stdio.h>

int main() {
	char scan_code;

	while (inportb(0x64) & 0x2 == 1) {
		printf("Controller 8042 is busy. Waiting...");
	}

	printf("Controller 8042 is not busy. Continue working...\n");

	outportb(0x60, 0xF3);
	outportb(0x60, 0xFC);
	printf("Values 0xF3 and 0xFC writed to port 0x60\n\n");

	printf("Writing values 0xED and 0x20 to port 0x60;\n");
	printf("ED is command code;\n");
	printf("20=00100000;\n");
	printf("0 - enable ScrollLock;\n");
	printf("0 - enable NumLock;\n");
	printf("1 - enable CapsLock;\n");
	printf("00000 are not using;\n");
	while (1) {
		outportb(0x60, 0xED);
		outportb(0x60, 0x20);
	}
	printf("Values 0xED and 0x20 writed to port 0x60.\n");

	while (scan_code != 16) {
		getch();
		scan_code = inportb(0x60);
        printf("Scan code of clicked button: %d\n", scan_code);
	}

	printf("Scan code is 16 ('q'). Stop the program.\n");

	printf("Returning to default keyboard settings. (Writing value 0xF6 to port 0x60)\n");
    outportb(0x60, 0xF6);

	return 0;            
}

