#include <dos.h>
#include <stdio.h>
#include <string.h>

#define bool int
#define true 1
#define false 0

#define SECTOR_SIZE 512

union REGS in, out;
struct SREGS sr;

struct datetime {
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned short year;
	unsigned char month;
	unsigned char day;
};

struct file_data {
	unsigned char name[8];
	unsigned char extension[3];
	unsigned char attributes;
	unsigned char reserve[8];
	unsigned short initial_cluster1_part;
	unsigned short time;
	unsigned short date;
	unsigned short initial_cluster2_part;
	unsigned long size;
};

char read_drive(char, char, char, int, unsigned char *);
void print_file(struct file_data);

int main() {
	int i;
	struct file_data files[16];
	char error_code;

	system("cls");

	/* No 1 */
	error_code = read_drive(28, 2, 24, 1, files);

	/* No 2 */
	printf("Error code = %d\n", error_code);

	/* No 3 */
	for (i = 0; i < 16; i++) {
		print_file(files[i]);
	}

	getch();

	return 0;
}

struct datetime words_to_datetime(unsigned short time_word, unsigned short date_word) {
	struct datetime new_date = {0, 0, 0};

	new_date.hour = (time_word & 0xF800) >> 11;
	new_date.minute = (time_word & 0x07E0) >> 5;
	new_date.second = (time_word & 0x001F) * 2;
	new_date.year = ((date_word & 0xFE00) >> 9) + 1980;
	new_date.month = (date_word & 0x01E0) >> 5;
	new_date.day = date_word & 0x001F;

	return new_date;
}

void print_file(struct file_data file) {
	struct datetime date = words_to_datetime(file.time, file.date);
	int i;

	printf("n='");
	for (i = 0; i < 8; i++) {
		printf("%c", file.name[i]);
	}
	printf("';e='");
	for (i = 0; i < 3; i++) {
		printf("%c", file.extension[i]);
	}
	printf("'; a='%0.2x'; ", file.attributes);
	printf("c='%0.4x%0.4x';", file.initial_cluster1_part, file.initial_cluster2_part);
	printf("t='%d.%0.2d.%0.2d", date.year, date.month, date.day);
	printf(" %0.2d:%0.2d:%0.2d';", date.hour, date.minute, date.second);
	printf("s='%u'", file.size);

	printf("\n");
}

char read_drive(char head, char cylinder, char sector, int sectors_count, unsigned char *buf) {
	in.h.ah = 2;
	in.h.dl = 0x80;
	in.h.dh = head;
	in.h.ch = cylinder;
	in.h.cl = sector; /* max = 63 */
	in.h.al = sectors_count;
	sr.es = FP_SEG(buf);
	in.x.bx = FP_OFF(buf);

	int86x(0x13, &in, &out, &sr);

	return out.h.ah;
}