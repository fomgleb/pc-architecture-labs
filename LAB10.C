#include <dos.h>
#include <stdio.h>

union REGS in, out;

void print_word(char *word, char word_settings, int x, int y) {
	char far *ptr = (char far *)0xB8000000;
	int i;

	for (i = 0; word[i] != '\0'; i++) {
		*(ptr + y * 160 + (x + i) * 2) = word[i];
		*(ptr + y * 160 + (x + i) * 2 + 1) = word_settings;
	}
}

int main() {
	in.h.al = 0x3;

	int86(0x10, &in, &out);

	print_word("I", 0xC, 11, 11);
	print_word("am", 0x5B, 13, 11);
	print_word("tired", 0xC, 16, 11);
	print_word("of", 0x5B, 22, 11);
	print_word("labs", 0x8C, 25, 11);

	getch();

	return 0;
}