#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

union REGS in, out;

void put_pix(int x, int y, char color);
void draw_line(int, int, int, int, char);
void draw_line2(int, int, int, int, char, int);
void fill_rect(int, int, int, int, char);
void draw_rect(int, int, int, int, int, char);
void draw_figure(struct point *, int, char);
void rotate_figure(struct point *, int, double, struct point);

struct point {
	double x;
	double y;
};

int main(void) {
	struct point rotation_point = {140, 70};
	struct point figure_points[3] = {{100, 100}, {150, 150}, {150, 50}};
	int i;
	double degrees = 1;

	/* No 1.1 */
	in.h.al = 0x10;
	int86(0x10, &in, &out);

	/* No 1.2 */
	/*fill_rect(0, 0, 200, 600, 0x1);
	fill_rect(200, 0, 500, 600, 0x2);
	fill_rect(500, 0, 700, 600, 0x6);*/

	/* No 1.6 */
	for (i = 0; i < 500; i++) {
		fill_rect(rotation_point.x - 1, rotation_point.y - 1, rotation_point.x + 1,
			rotation_point.y + 1, 2);
		rotate_figure(figure_points, 3, degrees, rotation_point);
		draw_figure(figure_points, 3, 1);
		delay(10);
		draw_figure(figure_points, 3, 0);
	}
	draw_figure(figure_points, 3, 1);

	getch();

	/* No 1.8 */
	in.h.al = 0x3;
	int86(0x10, &in, &out);

	getch();

	return 0;
}

void rotate_figure(struct point *points, int poins_count, double degrees,
struct point rotation_point) {
	int x, y;
	double radians = M_PI / 180.0 * degrees;
	double radians_cos = cos(radians);
	double radians_sin = sin(radians);
	double rotation_matrix[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 1}};
	double new_x, new_y;
	rotation_matrix[0][0] = radians_cos;
	rotation_matrix[0][1] = -radians_sin,
	rotation_matrix[0][2] = -rotation_point.x * radians_cos + rotation_point.y *
		radians_sin + rotation_point.x;
	rotation_matrix[1][0] = radians_sin;
	rotation_matrix[1][1] = radians_cos;
	rotation_matrix[1][2] = -rotation_point.x * radians_sin - rotation_point.y *
		radians_cos + rotation_point.y;

	for (y = 0; y < poins_count; y++) {
		new_x = rotation_matrix[0][0] * points[y].x + rotation_matrix[0][1]
			* points[y].y + rotation_matrix[0][2];
		new_y = rotation_matrix[1][0] * points[y].x + rotation_matrix[1][1]
			* points[y].y + rotation_matrix[1][2];
			points[y].x = new_x;
			points[y].y = new_y;
	}
}

void draw_figure(struct point *points, int points_count, char color) {
	int i;
	struct point first_point = {0, 0};
	struct point second_point = {0, 0};
	for (i = 0; i < points_count; i++) {
		first_point = points[i];
		second_point = i + 1 >= points_count ? points[0] : points[i + 1];
		draw_line(first_point.x, first_point.y,
			second_point.x, second_point.y, color);
	}
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
