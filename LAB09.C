#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	int i;

	system("cls");

	outportb(0x42, 1193180 / 494);
	outportb(0x42, 0);
	outportb(0x61, inportb(0x61) | 0x3);
	delay(500);
	outportb(0x42, 1193180 / 277);
	outportb(0x42, 0);
	delay(3000);
	outportb(0x42, 1193180 / 466);
	outportb(0x42, 0);
	delay(500);

	for (i = 0; i < 100; i++) {
		outportb(0x61, inportb(0x61) & 0xFC);
		delay(10);
		outportb(0x61, inportb(0x61) | 0x2);
		delay(10);
	}

	outportb(0x61, inportb(0x61) & 0xFC);

	getch();

	return 0;
}