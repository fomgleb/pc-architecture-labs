#include <dos.h>
#include <stdio.h>

int main() {
	char far *button_states = (char far *)0x00400017;

	system("cls");

	while (1) {
		if (*button_states == 8) {
			printf("Alt pressed\n");
			break;
		}
	}

	getch();

	return 0;

}