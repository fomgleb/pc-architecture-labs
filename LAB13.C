#include <stdio.h>
#include <dos.h>

union REGS in, out;

void put_pix(int x, int y, char color);

int main() {
	int i, j;
	char far *video_buffer = (char far *)0xB8000000;

	system("cls");

	/* No 1.1 */
	in.h.al = 0x10;
	int86(0x10, &in, &out);

	/* No 1.2 */
	in.h.ah = 0x10;
	in.h.al = 0x0;
	in.h.bh = 23;
	int86(0x10, &in, &out);

	/* No 1.3 */
	for (i = 10; i < 80; i++) {
		for (j = 10; j < 80; j++) {
			put_pix(i, j, 0x5);
		}
	}

	/* printf("jesus"); */
	getch();

	return 0;
}

/* No 2 */
void put_pix(int x, int y, char color) {
	union REGS f_in, f_out;
	f_in.h.ah = 0x0C;
	f_in.h.bh = 0;
	f_in.x.dx = y;
	f_in.x.cx = x;
	f_in.h.al = color;
	int86(0x10, &f_in, &f_out);
}