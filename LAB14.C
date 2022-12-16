#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

union REGS in, out;

void put_pix(int x, int y, char color);
void draw_line(int, int, int, int, char);
void draw_line2(int, int, int, int, char, int);
void fill_rect(int, int, int, int, char);
void draw_rect(int, int, int, int, int, char);

int main(void) {
	/* No 1.1 */
	in.h.al = 0x10;
	int86(0x10, &in, &out);

	/* No 1.2 */
	draw_rect(0, 0, 200, 150, 3, 1);

	/* No 1.3 */
	fill_rect(400, 60, 500, 120, 0xD);

	/* No 1.4 */
	draw_line2(30, 160, 500, 300, 0x32, 8);

	getch();

	/* No 1.5 */
	in.h.al = 0x3;
	int86(0x10, &in, &out);

	getch();

	return 0;
}

void draw_rect(int x1, int y1, int x2, int y2, int thickness, char color) {
	fill_rect(x1, y1, x2, y1 + thickness, color);
	fill_rect(x2 - thickness, y1, x2, y2, color);
	fill_rect(x1, y1, x1 + thickness, y2, color);
	fill_rect(x1, y2 - thickness, x2, y2, color);
}

void fill_rect(int x1, int y1, int x2, int y2, char color) {
	int x, y;
	for (x = x1; x < x2; x++) {
		for (y = y1; y < y2; y++) {
			put_pix(x, y, color);
		}
	}
}

void draw_line2(int x1, int y1, int x2, int y2, char color, int thickness) {
	int i;
	if (thickness == 1) {
		draw_line(x1, y1, x2, y2, color);
		return;
	}
	for (i = -(thickness / 2); i <= (thickness % 2 == 0 ? thickness / 2 - 1 : thickness / 2); i++) {
		draw_line(x1 + i, y1, x2 + i, y2, color);
	}
}

void draw_line(int x1, int y1, int x2, int y2, char color) {
	int delta_x = abs(x2 - x1);
	int delta_y = abs(y2 - y1);
	int sign_x = x1 < x2 ? 1 : -1;
	int sign_y = y1 < y2 ? 1 : -1;
	int error = delta_x - delta_y;
	int error2;
	put_pix(x2, y2, color);
	while(x1 != x2 || y1 != y2) {
		put_pix(x1, y1, color);
		error2 = error * 2;
		if (error2 > -delta_y) {
			error -= delta_y;
			x1 += sign_x;
		}
		if (error2 < delta_x) {
			error += delta_x;
			y1 += sign_y;
		}
	}
}

void put_pix(int x, int y, char color) {
	union REGS f_in, f_out;
	f_in.h.ah = 0x0C;
	f_in.h.bh = 0;
	f_in.x.dx = y;
	f_in.x.cx = x;
	f_in.h.al = color;
	int86(0x10, &f_in, &f_out);
}
